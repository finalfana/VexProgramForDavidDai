#pragma once
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "pros/motors.hpp"
#include "pros/imu.hpp"
#include "pros/optical.hpp"
#include "pros/adi.hpp"
#include <vector>
#include <cmath>

namespace RopoController
{
    enum AxisCastType
    {
        Linear,
        Exp,
        Ln,
        SlowLimitLinear
    };

    class AxisValueCast
    {
    private:
        pros::Controller &MyController;
        pros::controller_analog_e_t Axis;
        AxisCastType CastMode;
        double AxisDeadzone;;

    public:
        AxisValueCast(pros::Controller &_MyController,pros::controller_analog_e_t _Axis, AxisCastType _CastMode);
        float GetAxisValue();
        float GetDeadzone();
    };

    enum ButtonTriggerType
    {
        Rising,
        Falling,
        DoubleEdge,
        Pressing,
        DoubleClick,
    };

    class ButtonTaskCore
    {
    private:
        pros::controller_digital_e_t MyButton;
        ButtonTriggerType TriggerType;
        void *Callback;
        bool CallbackType;
        void *Parameter;
        int PressTime;
        pros::Task *BackgroundTask;
        bool Flag,DoubleClickFlag;
        int BeginTime;

        void Trigger();

    public:
        ButtonTaskCore(pros::controller_digital_e_t _Button, ButtonTriggerType _TriggerType, void (*_Callback)(void), int _PressTime);
        ButtonTaskCore(pros::controller_digital_e_t _Button, ButtonTriggerType _TriggerType, void (*_Callback)(void *), void *_Parameter, int _PressTime);
        ~ButtonTaskCore();
        pros::controller_digital_e_t Button();
        void Update(bool ButtonStatus);
    };
        
    class ButtonTaskLine
    {
    private:
        pros::Controller &MyController;
        std::vector<ButtonTaskCore> ButtonQueue;
        pros::Task *BackgroundTask;

        static void BackgroundTaskFunction(void *Parameter);

    public:
        ButtonTaskLine(pros::Controller&_MyController);
        ~ButtonTaskLine();
        void Enable();
        void AddButtonDetect(pros::controller_digital_e_t Button,ButtonTriggerType TriggerType, void (*Callback)(void), int PressTime = 1000);
        void AddButtonDetect(pros::controller_digital_e_t Button,ButtonTriggerType TriggerType, void (*Callback)(void *), void *Parameter, int PressTime = 1000);
    };
}

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
        ChassisAutonomous(pros::MotorGroup &_LeftMotors, pros::MotorGroup &_RightMotors, pros::IMU &_Gyro);
        void Initialize();
        void PrintLog();
    };
}

namespace Collector
{
    enum SelectColor
    {
        NONE,
        RED,
        BLUE,
    };

    struct ColorThresholds 
    {
        double sat_min;
        double bri_min;
        double bri_max;
    };

    struct DetectedColorData
    {
        double hue;
        double sat;
        double bri;
        SelectColor detected_color; 
    };

    class CollectorControl
    {
    private:
        pros::MotorGroup &LiftMotors;
        pros::Motor &SelectMotor;
        pros::Optical SelectColorSensor;
        SelectColor MyColor, EnemyColor;
        ColorThresholds CollectorColorThresholds;
        const int ColorDetectWindow;
        const int ColorDetectInterval;
        std::unique_ptr<pros::Task> ColorDetectTask;
        std::vector<DetectedColorData> ColorRecords;

        static void DetectFunction(void* Parameter);

    public:
        CollectorControl(pros::MotorGroup &_LiftMotors, pros::Motor &_SelectMotor, pros::Optical &_SelectColorSensor, SelectColor _MyColor, SelectColor _EnemyColor);
        static void LiftUp(void* Para);
        static void Brake(void* Para);
        DetectedColorData GetColorRecordAverage();
        void ColorDetect();
        void EnableColorDetect();
    };
}

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
        ChassisOpControl(pros::MotorGroup &_LeftMotors, pros::MotorGroup &_RightMotors, RopoController::AxisValueCast &_LeftAxisX, RopoController::AxisValueCast &_LeftAxisY);
        void DetectAndMove();
    };

    class ScraperOpControl
    {
    private:
        pros::ADIDigitalOut &ScraperPiston;

    public:
        ScraperOpControl(pros::ADIDigitalOut &_ScraperPiston);
        static void LiftScraper(void* Parameter);
        static void LowerScraper(void* Parameter);
    };

}