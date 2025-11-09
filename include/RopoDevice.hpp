#pragma once 
#include "pros/motors.h"
#include "pros/motors.hpp"

namespace Motors
{
    enum MotorDirection
    {
        NORMAL = 0,
        REVERSED = 1
    };

    enum MotorPort
    {
        LEFTBACKMOTORPORT = 1,
        LEFTFRONTMOTORPORT = 2,
        RIGHTBACKMOTORPORT = 3,
        RIGHTFRONTMOTORPORT = 4,
    };

    pros::Motor LeftBackMotor(LEFTBACKMOTORPORT, pros::E_MOTOR_GEARSET_06, NORMAL); 
    pros::Motor LeftFrontMotor(LEFTFRONTMOTORPORT, pros::E_MOTOR_GEARSET_06, NORMAL); 
    pros::Motor RightBackMotor(RIGHTBACKMOTORPORT, pros::E_MOTOR_GEARSET_06, REVERSED); 
    pros::Motor RightFrontMotor(RIGHTFRONTMOTORPORT, pros::E_MOTOR_GEARSET_06, REVERSED);
    
    pros::MotorGroup LeftMotors({LeftBackMotor, LeftFrontMotor});
    pros::MotorGroup RightMotors({RightBackMotor, RightFrontMotor});
}

