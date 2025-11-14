#include "Devices.hpp"
#include "pros/motors.hpp"
#include "pros/adi.hpp"

namespace Motors
{

    pros::Motor LeftBackMotor(1, pros::E_MOTOR_GEARSET_06, NORMAL, pros::E_MOTOR_ENCODER_DEGREES); //待定数据
    pros::Motor LeftFrontMotor(2, pros::E_MOTOR_GEARSET_06, NORMAL, pros::E_MOTOR_ENCODER_DEGREES); //待定数据
    pros::Motor RightBackMotor(3, pros::E_MOTOR_GEARSET_06, REVERSED, pros::E_MOTOR_ENCODER_DEGREES); //待定数据
    pros::Motor RightFrontMotor(4, pros::E_MOTOR_GEARSET_06, REVERSED, pros::E_MOTOR_ENCODER_DEGREES);//待定数据
    pros::Motor LiftMotor1(5, pros::E_MOTOR_GEARSET_18, NORMAL, pros::E_MOTOR_ENCODER_DEGREES);//待定数据
    pros::Motor LiftMotor2(6, pros::E_MOTOR_GEARSET_18, NORMAL, pros::E_MOTOR_ENCODER_DEGREES);//待定数据
    pros::Motor LiftMotor3(7, pros::E_MOTOR_GEARSET_18, NORMAL, pros::E_MOTOR_ENCODER_DEGREES);//待定数据
    pros::Motor SelectMotor(8, pros::E_MOTOR_GEARSET_06, NORMAL, pros::E_MOTOR_ENCODER_DEGREES);//待定数据
    
    pros::MotorGroup LeftMotors({LeftBackMotor, LeftFrontMotor});
    pros::MotorGroup RightMotors({RightBackMotor, RightFrontMotor});
    pros::MotorGroup LiftMotors({LiftMotor1, LiftMotor2, LiftMotor3});

    pros::ADIDigitalOut ScraperPiston('A'); //待定数据
}

