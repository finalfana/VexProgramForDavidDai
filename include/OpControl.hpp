#pragma once
#include "pros/motors.hpp"
#include "RopoController.hpp"
#include <cstdlib>


namespace OpControl
{
    class ChassisOpControl
    {
    private:
        pros::MotorGroup &LeftMotors;
        pros::MotorGroup &RightMotors;
        RopoController::AxisValueCast &LeftAxisX;
        RopoController::AxisValueCast &LeftAxisY;

    public:
        ChassisOpControl(pros::MotorGroup &_LeftMotors, pros::MotorGroup &_RightMotors, RopoController::AxisValueCast &_LeftAxisX, RopoController::AxisValueCast &_LeftAxisY)
            : LeftMotors(_LeftMotors), RightMotors(_RightMotors), LeftAxisX(_LeftAxisX), LeftAxisY(_LeftAxisY) {}

        void DetectAndMove()
        {
            if (std::fabs(LeftAxisX.GetAxisValue()) <= LeftAxisX.GetDeadzone() && std::fabs(LeftAxisY.GetAxisValue()) <= LeftAxisY.GetDeadzone())
            {
                LeftMotors.brake();
                RightMotors.brake();
            }

            else 
            {
                double LeftSpeed = LeftAxisY.GetAxisValue() + LeftAxisX.GetAxisValue();
                double RightSpeed = LeftAxisY.GetAxisValue() - LeftAxisX.GetAxisValue();

                LeftMotors.move_velocity(LeftSpeed * 600);//待定数据
                RightMotors.move_velocity(RightSpeed * 600);//待定数据
            }
        }

    };
}