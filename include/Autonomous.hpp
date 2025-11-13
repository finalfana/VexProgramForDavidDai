#pragma once
#include "pros/motors.hpp"
#include "pros/imu.hpp"
#include <iostream>

namespace Autonomous
{
    class ChassisAutonomous
    {
    private:
        pros::MotorGroup &LeftMotors;
        pros::MotorGroup &RightMotors;
        pros::IMU &Gyro;
        double PositionX, PositionY, Angle;
        double BeginTime;

    public:
        ChassisAutonomous(pros::MotorGroup &_LeftMotors, pros::MotorGroup &_RightMotors, pros::IMU &_Gyro)
            : LeftMotors(_LeftMotors), RightMotors(_RightMotors), Gyro(_Gyro), PositionX(0), PositionY(0), Angle(0) 
        {
            BeginTime = pros::millis();
        }
        
        void Initialize()
        {
            Gyro.reset(true);
        }

        void PrintLog()
        {
            double CurrentTime = pros::millis() - BeginTime;
            std::cout << "PositionX: " << PositionX << " PositionY: " << PositionY << " Angle: " << Angle << "CurrentTime: " << CurrentTime << std::endl;
        }

        
    };
}