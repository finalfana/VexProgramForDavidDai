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
        LEFTBACKMOTORPORT = 1,//待定数据
        LEFTFRONTMOTORPORT = 2,//待定数据
        RIGHTBACKMOTORPORT = 3,//待定数据
        RIGHTFRONTMOTORPORT = 4,//待定数据
        LIFTMOTOR1PORT = 5,//待定数据
        LIFTMOTOR2PORT = 6,//待定数据
        LIFTMOTOR3PORT = 7,//待定数据
        SELECTMOTORPORT = 8,//待定数据
    };

    pros::Motor LeftBackMotor(LEFTBACKMOTORPORT, pros::E_MOTOR_GEARSET_06, NORMAL, pros::E_MOTOR_ENCODER_DEGREES); //待定数据
    pros::Motor LeftFrontMotor(LEFTFRONTMOTORPORT, pros::E_MOTOR_GEARSET_06, NORMAL, pros::E_MOTOR_ENCODER_DEGREES); //待定数据
    pros::Motor RightBackMotor(RIGHTBACKMOTORPORT, pros::E_MOTOR_GEARSET_06, REVERSED, pros::E_MOTOR_ENCODER_DEGREES); //待定数据
    pros::Motor RightFrontMotor(RIGHTFRONTMOTORPORT, pros::E_MOTOR_GEARSET_06, REVERSED, pros::E_MOTOR_ENCODER_DEGREES);//待定数据
    pros::Motor LiftMotor1(LIFTMOTOR1PORT, pros::E_MOTOR_GEARSET_18, NORMAL, pros::E_MOTOR_ENCODER_DEGREES);//待定数据
    pros::Motor LiftMotor2(LIFTMOTOR2PORT, pros::E_MOTOR_GEARSET_18, NORMAL, pros::E_MOTOR_ENCODER_DEGREES);//待定数据
    pros::Motor LiftMotor3(LIFTMOTOR3PORT, pros::E_MOTOR_GEARSET_18, NORMAL, pros::E_MOTOR_ENCODER_DEGREES);//待定数据
    pros::Motor SelectMotor(SELECTMOTORPORT, pros::E_MOTOR_GEARSET_06, NORMAL, pros::E_MOTOR_ENCODER_DEGREES);//待定数据
    
    pros::MotorGroup LeftMotors({LeftBackMotor, LeftFrontMotor});
    pros::MotorGroup RightMotors({RightBackMotor, RightFrontMotor});
    pros::MotorGroup LiftMotors({LiftMotor1, LiftMotor2, LiftMotor3});

}

