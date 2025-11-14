#include "project.hpp"
#include "pros/motors.hpp"
#include "pros/imu.hpp"
#include <iostream>

namespace Autonomous
{
    ChassisAutonomous::ChassisAutonomous(pros::MotorGroup &_LeftMotors, pros::MotorGroup &_RightMotors, pros::IMU &_Gyro)
        : LeftMotors(_LeftMotors), RightMotors(_RightMotors), Gyro(_Gyro), PositionX(0), PositionY(0), Angle(0) 
    {
        BeginTime = pros::millis();
    }
    
    void ChassisAutonomous::Initialize()
    {
        Gyro.reset(true);
    }

    void ChassisAutonomous::PrintLog()
    {
        double CurrentTime = pros::millis() - BeginTime;
        std::cout << "PositionX: " << PositionX << " PositionY: " << PositionY << " Angle: " << Angle << "CurrentTime: " << CurrentTime << std::endl;
    }
}