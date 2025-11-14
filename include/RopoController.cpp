#include "project.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include <vector>
#include <cmath>

namespace RopoController
{
    AxisValueCast::AxisValueCast(pros::Controller &_MyController,pros::controller_analog_e_t _Axis, AxisCastType _CastMode)
        : MyController(_MyController), Axis(_Axis), CastMode(_CastMode), AxisDeadzone(0.05){}

    float AxisValueCast::GetAxisValue()
    {
        float AxisValue = MyController.get_analog(Axis);
        AxisValue /= 127.0;

        if(CastMode == Linear) 
            return AxisValue;

        else if (CastMode == Exp) 
        { 
            if(AxisValue > 0)
                AxisValue = (exp(AxisValue) - 1) / (exp(1)-1) ;
            else
                AxisValue = - ((exp(-AxisValue) - 1) / (exp(1)-1) );

            return AxisValue;
        }

        else if(CastMode == Ln)
        { 
            if(AxisValue > 0) AxisValue = log(AxisValue+1) / log(2);

            else AxisValue = - log(-AxisValue+1) / log(2);

            return AxisValue;
        }

        else if(CastMode == SlowLimitLinear)
            return 0.75 * AxisValue;

        return AxisValue;
    }

    float AxisValueCast::GetDeadzone()
    {
        if (CastMode == Linear || CastMode == SlowLimitLinear)
            return AxisDeadzone;

        else if (CastMode == Exp)
            return (exp(AxisDeadzone) - 1) / (exp(1)-1);

        else if (CastMode == Ln)
            return log(AxisDeadzone + 1) / log(2);
        return AxisDeadzone;
    }

    void ButtonTaskCore::Trigger()
    {
        if(!CallbackType)
        {
            delete BackgroundTask;
            BackgroundTask = new pros::Task((void (*)(void))Callback);
        }

        else 
        {
            delete BackgroundTask;
            BackgroundTask = new pros::Task((void (*)(void*))Callback,Parameter);
        }
    }

    ButtonTaskCore::ButtonTaskCore(pros::controller_digital_e_t _Button, ButtonTriggerType _TriggerType, void (*_Callback)(void), int _PressTime)
        : MyButton(_Button),
          TriggerType(_TriggerType),
          Callback(nullptr),
          CallbackType(false),
          Parameter(nullptr),
          PressTime(_PressTime),
          BackgroundTask(nullptr),
          Flag(false),
          DoubleClickFlag(false),
          BeginTime(-1)
    {
        if(TriggerType == Falling)
        {
            Flag = true;
        }

        Callback = (void *)_Callback;
    }

    ButtonTaskCore::ButtonTaskCore(pros::controller_digital_e_t _Button, ButtonTriggerType _TriggerType, void (*_Callback)(void *), void *_Parameter, int _PressTime)
        : MyButton(_Button),
          TriggerType(_TriggerType),
          Callback(nullptr),
          CallbackType(true),
          Parameter(_Parameter),
          PressTime(_PressTime),
          BackgroundTask(nullptr),
          Flag(false),
          DoubleClickFlag(false),
          BeginTime(-1)
    {
        if(TriggerType == Falling)
        {
        Flag = true;
        }

        Callback = (void *)_Callback;
    }

    ButtonTaskCore::~ButtonTaskCore() {delete BackgroundTask;}

    pros::controller_digital_e_t ButtonTaskCore::Button()
    {
        return MyButton;
    }

    void ButtonTaskCore::Update(bool ButtonStatus)
    {
        if( TriggerType == Rising )
        {
            if(Flag == false && ButtonStatus == true)
            {
                Flag = true;
                Trigger();
            }

            else if(ButtonStatus == false)
            {
                Flag = false;
            }
        }

        else if( TriggerType == Falling )
        {
            if(Flag == false && ButtonStatus == false)
            {
                Flag = true;
                Trigger();
            }

            else if(ButtonStatus == true)
            {
                Flag = false;
            }
        }

        else if( TriggerType == DoubleEdge )
        {
            if(Flag == false && ButtonStatus == true)
            {
                Flag = true;
                Trigger();
            }

            else if(Flag == true && ButtonStatus == false) 
            {
                Flag = false;
                Trigger();
            }
        }

        else if( TriggerType == Pressing )
        {
            if(Flag == false && ButtonStatus == true) 
            {
                Flag = true, BeginTime = pros::millis();
            }

            else if(ButtonStatus == false) 
            {
                Flag = false, BeginTime = -1;
            }

            else if(BeginTime != -1 && ButtonStatus == true && pros::millis() > BeginTime + PressTime)
            {
                Trigger();
                BeginTime = -1;
            }
        }

        else if( TriggerType == DoubleClick )
        {
            if(BeginTime + 200 < pros::millis())
            {
                BeginTime = -1, DoubleClickFlag = false;
            }

            if(Flag == false && DoubleClickFlag == false && ButtonStatus== true)
            {
                BeginTime = pros::millis(), Flag = true;
            }
            
            else if(Flag == true && DoubleClickFlag == false && BeginTime!= -1 && ButtonStatus == false)
            {
                DoubleClickFlag = true;
            }
            else if(Flag == true && DoubleClickFlag == true && BeginTime!= -1 && ButtonStatus == true)
            {
                Trigger();
                BeginTime = -1;
            }

            else if(BeginTime == -1) Flag = false;
        }
    }

    void ButtonTaskLine::BackgroundTaskFunction(void *Parameter)
    {
        if(Parameter == nullptr) return;
        ButtonTaskLine *This = static_cast<ButtonTaskLine *>(Parameter);  

        while(true)
        {
            for(ButtonTaskCore &ButtonTask : This->ButtonQueue)
            {
                bool ButtonStatus = This -> MyController.get_digital(ButtonTask.Button());
                ButtonTask.Update(ButtonStatus);
            }
            
            pros::delay(20);
        }
    }

    ButtonTaskLine::ButtonTaskLine(pros::Controller&_MyController)
            : MyController(_MyController) {BackgroundTask = nullptr;}

    ButtonTaskLine::~ButtonTaskLine()
    {
        delete BackgroundTask;
        ButtonQueue.clear();
    }

    void ButtonTaskLine::Enable()
    {
        BackgroundTask = new pros::Task(BackgroundTaskFunction,this);
    }

    void ButtonTaskLine::AddButtonDetect(pros::controller_digital_e_t Button,ButtonTriggerType TriggerType, void (*Callback)(void), int PressTime)
    {
        ButtonTaskCore *temp = new ButtonTaskCore(Button,TriggerType,Callback,PressTime);
        ButtonQueue.push_back(*temp);
    }
    
    void ButtonTaskLine::AddButtonDetect(pros::controller_digital_e_t Button,ButtonTriggerType TriggerType, void (*Callback)(void *), void *Parameter, int PressTime)
    {
        ButtonTaskCore *temp = new ButtonTaskCore(Button,TriggerType,Callback,Parameter,PressTime);
        ButtonQueue.push_back(*temp);
    }
}