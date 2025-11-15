#include "Project.hpp"
#include "pros/motors.hpp"
#include "pros/adi.hpp"
#include <cstdlib>


namespace OpControl
{
    ChassisOpControl::ChassisOpControl(pros::MotorGroup &_LeftMotors, pros::MotorGroup &_RightMotors, RopoController::AxisValueCast &_LeftAxisX, RopoController::AxisValueCast &_LeftAxisY)
        : LeftMotors(_LeftMotors), RightMotors(_RightMotors), LeftAxisX(_LeftAxisX), LeftAxisY(_LeftAxisY) {}

    void ChassisOpControl::DetectAndMove()
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

    ScraperOpControl::ScraperOpControl(pros::ADIDigitalOut &_ScraperPiston)
        : ScraperPiston(_ScraperPiston) {}

    void ScraperOpControl::LiftScraper(void* Parameter)
    {
        if (Parameter == nullptr) return;
        ScraperOpControl *This = static_cast<ScraperOpControl*>(Parameter);
        This -> ScraperPiston.set_value(true);
    }

    void ScraperOpControl::LowerScraper(void* Parameter)
    {
        if (Parameter == nullptr) return;
        ScraperOpControl *This = static_cast<ScraperOpControl*>(Parameter);
        This -> ScraperPiston.set_value(false);
    }
}