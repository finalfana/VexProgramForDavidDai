#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include "pros/adi.hpp"

namespace Motors
{
    enum MotorDirection
    {
        NORMAL = 0,
        REVERSED = 1
    };

    extern pros::Motor LeftBackMotor;
    extern pros::Motor LeftFrontMotor;
    extern pros::Motor RightBackMotor; 
    extern pros::Motor RightFrontMotor;
    extern pros::Motor LiftMotor1;
    extern pros::Motor LiftMotor2;
    extern pros::Motor LiftMotor3;
    extern pros::Motor SelectMotor;
    
    extern pros::MotorGroup LeftMotors;
    extern pros::MotorGroup RightMotors;
    extern pros::MotorGroup LiftMotors;
}

namespace ThreeWires
{
    extern pros::ADIDigitalOut ScraperPiston; 
}

namespace Sensors
{
    extern pros::IMU GyroSensor;
}
