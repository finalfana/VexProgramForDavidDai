#include "project.hpp"
#include "pros/motors.hpp"
#include "pros/imu.hpp"
#include <cmath>
#include <iostream>

namespace Autonomous
{
    PID::PID(double _kP, double _kI, double _kD, double _integral_limit)
        : kP(_kP), kI(_kI), kD(_kD), integral_limit(_integral_limit) {}

    ChassisAutonomous::ChassisAutonomous(pros::MotorGroup &_LeftMotors, pros::MotorGroup &_RightMotors, pros::IMU &_Gyro)
        : LeftMotors(_LeftMotors), RightMotors(_RightMotors), Gyro(_Gyro), MyPosition{0.0,0.0,0.0}
    {
        BeginTime = pros::millis();
    }
    
    void ChassisAutonomous::Initialize()
    {
        Gyro.reset(true);
    }

    double ChassisAutonomous::DegreeToRad(double degree)
    {
        return degree * M_PI / 180;
    }

    double ChassisAutonomous::RadToDegree(double rad)
    {
        return rad * 180 / M_PI;
    }

    double ChassisAutonomous::GetCurrentAngle()
    {
        double HeadingDegree = NormalizeAngle((-Gyro.get_heading()));
        return HeadingDegree;
    }

    double ChassisAutonomous::NormalizeAngle(double angle)
    {
        while (angle >= 180.0) angle -= 360.0;
        while (angle < -180.0) angle += 360.0;
        return angle;
    }

    double ChassisAutonomous::DegreeToDistance(double degree) 
    {
        double rotation = degree / 360.0;
        return rotation * RopoConfig::WheelCircumference;
    }

    double ChassisAutonomous::DistanceToDegree(double distance) 
    {
        double rotation = distance / RopoConfig::WheelCircumference;
        return rotation * 360.0;
    }

    void ChassisAutonomous::MoveDistance(double TargetDistance)
    {
        static const PID PositionPID(1.0, 0.0, 0.0, 100.0); //待定数据
        static const int dt = 10;//积分间隔,待定数据
        static const int PosStableThreshold = 5;//连续检测到位置数据在误差允许范围内的次数，待定数据   
        static const int PosTimeOut = 7000;//待定数据

        double PreDistance = 0.0;
        double PreError = 0.0;
        double Integral = 0.0;

        int StableCount = 0;
        int BeginTime = pros::millis();

        LeftMotors.tare_position();
        RightMotors.tare_position();

        while (pros::millis() - BeginTime < PosTimeOut)
        {
            std::vector<double> LeftDegree = LeftMotors.get_positions();
            std::vector<double> RightDegree = RightMotors.get_positions();

            double LeftAvgDegree = (LeftDegree[0] + LeftDegree[1]) / 2;
            double RightAvgDegree = (RightDegree[0] + RightDegree[1]) / 2;
            double AvgDegree = (LeftAvgDegree + RightAvgDegree) / 2;   

            double CurrentDistance = DegreeToDistance(AvgDegree);
            double CurrentError = TargetDistance - CurrentDistance;

            // 更新位置信息
            MyPosition.angle = GetCurrentAngle();
            MyPosition.x += (CurrentDistance - PreDistance) * std::cos(DegreeToRad(MyPosition.angle));
            MyPosition.y += (CurrentDistance - PreDistance) * std::sin(DegreeToRad(MyPosition.angle));

            Integral += CurrentError * (dt / 1000.0);

            // 积分限制
            if (std::fabs(Integral) > PositionPID.integral_limit)
                Integral = (Integral > 0) ? PositionPID.integral_limit : -PositionPID.integral_limit;

            double Derivative = (CurrentError - PreError) / (dt / 1000.0);
            PreError = CurrentError;
            PreDistance = CurrentDistance;

            double OutputPower = PositionPID.kP * CurrentError + PositionPID.kI * Integral + PositionPID.kD * Derivative;

            // 功率限制
            if (std::fabs(OutputPower) > RopoConfig::MaxPower)
                OutputPower = (OutputPower > 0) ? RopoConfig::MaxPower : -RopoConfig::MaxPower;

            LeftMotors.move(static_cast<int>(OutputPower));
            RightMotors.move(static_cast<int>(OutputPower));

            if (std::fabs(CurrentError) <= RopoConfig::PosTolerance)
                StableCount++;
            else StableCount = 0;

            if (StableCount >= PosStableThreshold) break;

            pros::delay(dt);
        }

        LeftMotors.brake();
        RightMotors.brake();
    }

    void ChassisAutonomous::MoveAngleAbsolute(double TargetAngle)
    {
        static const PID AnglePID(1.0, 0.0, 0.0, 100.0); //待定数据
        static const int dt = 10;//待定数据
        static const int AngleStableThreshold = 5;//待定数据
        static const int AngleTimeOut = 7000;//待定数据

        MyPosition.angle = GetCurrentAngle();
        double PreError = 0.0;
        double Integral = 0.0;

        int StableCount = 0;
        int BeginTime = pros::millis();

        // 防呆
        while (Gyro.is_calibrating()) 
        {
            pros::delay(10);
        }

        // 规范化角度到[-180,180)范围
        TargetAngle = NormalizeAngle(TargetAngle);

        while (pros::millis() - BeginTime < AngleTimeOut)
        {
            MyPosition.angle = GetCurrentAngle();
            double CurrentAngle = NormalizeAngle(MyPosition.angle);
            double CurrentError = TargetAngle - CurrentAngle;

            Integral += CurrentError * (dt / 1000.0);

            // 积分限制
            if (std::fabs(Integral) > AnglePID.integral_limit)
                Integral = (Integral > 0) ? AnglePID.integral_limit : -AnglePID.integral_limit;

            double Derivative = (CurrentError - PreError) / (dt / 1000.0);
            PreError = CurrentError;

            double OutputPower = AnglePID.kP * CurrentError + AnglePID.kI * Integral + AnglePID.kD * Derivative;

            // 功率限制
            if (std::fabs(OutputPower) > RopoConfig::MaxPower)
                OutputPower = (OutputPower > 0) ? RopoConfig::MaxPower : -RopoConfig::MaxPower;

            LeftMotors.move(static_cast<int>(OutputPower));
            RightMotors.move(-static_cast<int>(OutputPower));

            if (std::fabs(CurrentError) <= RopoConfig::AngleTolerance)
                StableCount++;
            else StableCount = 0;

            if (StableCount >= AngleStableThreshold) break;

            pros::delay(dt);
        }
    
        LeftMotors.brake();
        RightMotors.brake();
    }

    void ChassisAutonomous::MovePositionAbsolute(double TargetX, double TargetY)
    {
        double dx = TargetX - MyPosition.x;
        double dy = TargetY - MyPosition.y;
        double distance = std::sqrt(dx * dx + dy * dy);

        if (distance < RopoConfig::PosTolerance) return;

        double Theta = RadToDegree(std::atan2(dy, dx));

        MoveAngleAbsolute(Theta);
        MoveDistance(distance);

    }


    void ChassisAutonomous::PrintLog()
    {
        double CurrentTime = pros::millis() - BeginTime;
        std::cout << "PositionX: " << MyPosition.x 
                  << " PositionY: " << MyPosition.y 
                  << " Angle: " << MyPosition.angle 
                  << "CurrentTime: " << CurrentTime << std::endl;
    }
}