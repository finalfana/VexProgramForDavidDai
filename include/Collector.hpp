#include "pros/motors.hpp"
#include "pros/optical.hpp"

namespace Collector
{
    class CollectorControl
    {
    private:
        pros::MotorGroup &LiftMotors;
        pros::Motor &SelectMotor;
        pros::Optical SelectColorSensor;

    public:
        CollectorControl(pros::MotorGroup &_LiftMotors, pros::Motor &_SelectMotor, pros::Optical &_SelectColorSensor)
            : LiftMotors(_LiftMotors), SelectMotor(_SelectMotor), SelectColorSensor(_SelectColorSensor) {}

        void LiftUp()
        {
            LiftMotors.move_velocity(200);
            SelectMotor.move_velocity(200);
        }

        void Brake()
        {
            LiftMotors.brake();
            SelectMotor.brake();
        }


    };
}