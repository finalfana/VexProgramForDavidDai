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
        LIFTMOTOR1PORT = 5,
        LIFTMOTOR2PORT = 6,
        LIFTMOTOR3PORT = 7,
        SELECTMOTORPORT = 8,
    };

    pros::Motor LeftBackMotor(LEFTBACKMOTORPORT, pros::E_MOTOR_GEARSET_06, NORMAL, pros::E_MOTOR_ENCODER_DEGREES); 
    pros::Motor LeftFrontMotor(LEFTFRONTMOTORPORT, pros::E_MOTOR_GEARSET_06, NORMAL, pros::E_MOTOR_ENCODER_DEGREES); 
    pros::Motor RightBackMotor(RIGHTBACKMOTORPORT, pros::E_MOTOR_GEARSET_06, REVERSED, pros::E_MOTOR_ENCODER_DEGREES); 
    pros::Motor RightFrontMotor(RIGHTFRONTMOTORPORT, pros::E_MOTOR_GEARSET_06, REVERSED, pros::E_MOTOR_ENCODER_DEGREES);
    pros::Motor LiftMotor1(LIFTMOTOR1PORT, pros::E_MOTOR_GEARSET_18, NORMAL, pros::E_MOTOR_ENCODER_DEGREES);
    pros::Motor LiftMotor2(LIFTMOTOR2PORT, pros::E_MOTOR_GEARSET_18, NORMAL, pros::E_MOTOR_ENCODER_DEGREES);
    pros::Motor LiftMotor3(LIFTMOTOR3PORT, pros::E_MOTOR_GEARSET_18, NORMAL, pros::E_MOTOR_ENCODER_DEGREES);
    pros::Motor SelectMotor(SELECTMOTORPORT, pros::E_MOTOR_GEARSET_06, NORMAL, pros::E_MOTOR_ENCODER_DEGREES);
    
    pros::MotorGroup LeftMotors({LeftBackMotor, LeftFrontMotor});
    pros::MotorGroup RightMotors({RightBackMotor, RightFrontMotor});
    pros::MotorGroup LiftMotors({LiftMotor1, LiftMotor2, LiftMotor3});

}

