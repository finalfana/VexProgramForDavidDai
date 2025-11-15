#include "Project.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/rtos.hpp"
#include <vector>
#include <cmath>
#include <memory>


namespace Collector
{
    void CollectorControl::DetectFunction(void* Parameter)
    {
        if (Parameter == nullptr) return;
        CollectorControl *This = static_cast<CollectorControl*>(Parameter);

        while (true)
        {
            This -> ColorDetect();
            pros::delay(This -> ColorDetectInterval);
        }
    }

    CollectorControl::CollectorControl(pros::MotorGroup &_LiftMotors, pros::Motor &_SelectMotor, pros::Optical &_SelectColorSensor, SelectColor _MyColor, SelectColor _EnemyColor)
        : LiftMotors(_LiftMotors),
          SelectMotor(_SelectMotor), 
          SelectColorSensor(_SelectColorSensor), 
          MyColor(_MyColor), 
          EnemyColor(_EnemyColor),
          ColorDetectWindow(10),
          ColorDetectInterval(100),
          CollectorColorThresholds{20.0, 5.0, 95.0} //待定数据
    {
        ColorRecords.reserve(ColorDetectWindow);
    }

        void CollectorControl::LiftUp(void* Parameter)
        {
            if (Parameter == nullptr) return;
            CollectorControl *This = static_cast<CollectorControl*>(Parameter);
            This -> LiftMotors.move_velocity(200);//待定数据
            This -> SelectMotor.move_velocity(200);//待定数据
        }

        void CollectorControl::Brake(void* Parameter)
        {
            if (Parameter == nullptr) return;
            CollectorControl *This = static_cast<CollectorControl*>(Parameter);
            This -> LiftMotors.brake();
            This -> SelectMotor.brake();
        }

    DetectedColorData CollectorControl::GetColorRecordAverage()
    {
        double hue_sum = 0.0;
        double sat_sum = 0.0;
        double bri_sum = 0.0;
        double cos_sum = 0.0;
        double sin_sum = 0.0;

        if (ColorRecords.empty())
        {
            return {0.0, 0.0, 0.0, NONE};
        }

        for  (DetectedColorData &record : ColorRecords)
        {
            sat_sum += record.sat;
            bri_sum += record.bri;

            double hue_rad = record.hue * (M_PI / 180.0);
            cos_sum += std::cos(hue_rad);
            sin_sum += std::sin(hue_rad);
        }

        double hue_avg_rad = std::atan2(sin_sum, cos_sum);
        double hue_avg_deg = hue_avg_rad * 180.0 / M_PI;
        if (hue_avg_deg < 0) hue_avg_deg += 360.0;

        // 这个函数不负责判断是否检测到物体，统一返回NONE
        return {hue_avg_deg, sat_sum / ColorRecords.size(), bri_sum / ColorRecords.size(), NONE};
    }
        
    void CollectorControl::ColorDetect()
    {
        float hue = SelectColorSensor.get_hue();
        float sat = SelectColorSensor.get_saturation();
        float bri = SelectColorSensor.get_brightness();

        // 如果采样没有满，统一判断没有检测到物体
        if (ColorRecords.size() <= ColorDetectWindow)
        {
            ColorRecords.push_back({hue, sat, bri, NONE});
        }

        else 
        {
            ColorRecords.erase(ColorRecords.begin());
            ColorRecords.push_back({hue, sat, bri, NONE});
            
            DetectedColorData AvgColor = GetColorRecordAverage();
            // 判断是否检测到物体
            bool satisfied = (AvgColor.sat >= CollectorColorThresholds.sat_min) && (AvgColor.bri >= CollectorColorThresholds.bri_min) && (AvgColor.bri <= CollectorColorThresholds.bri_max);

            if (satisfied == true)
            {
                if (AvgColor.hue >= 350 || AvgColor.hue <= 20)//待定数据
                {
                    ColorRecords[-1].detected_color = RED;
                }

                else if (AvgColor.hue >= 200 && AvgColor.hue <= 240)//待定数据
                {
                    ColorRecords[-1].detected_color = BLUE;
                }

                else 
                {
                    ColorRecords[-1].detected_color = NONE;
                }
            }

        }
    }

    void CollectorControl::EnableColorDetect()
    {
        if (!ColorDetectTask) ColorDetectTask = std::make_unique<pros::Task>(pros::Task(DetectFunction, this));
    }
}