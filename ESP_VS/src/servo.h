
#ifndef __SERVO_h
#define __SERVO_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class Servo
{
  public:
    Servo(uint16_t pin, uint16_t channel, uint16_t frequency)
      : pin_(pin)
      , channel_(channel)
      , frequency_(frequency)
      , resolution_(16) // Default to 16 bit resolution

    {
    }

    void Begin()
    {
        ledcSetup(channel_, frequency_, resolution_);
        ledcAttachPin(pin_, channel_);
    }

  protected:
    uint16_t pin_, channel_, frequency_;
    const uint8_t resolution_;
};

template <typename T>
T clamp(T value, T min, T max)
{
    if (value > max)
        return max;
    else if (value < min)
        return min;
    else
        return value;
}

template <typename T>
int sgn(T val)
{
    if (val >= T(0))
        return 1;
    else
        return -1;
}

class steeringServo : public Servo
{
  public:
    steeringServo(uint16_t pin, uint16_t channel, int8_t lower_angle_limit = -90, int8_t upper_angle_limit = 90,
                  int8_t centering_offset = 0)
      : Servo(pin, channel, 50)
      , lower_angle_limit_(lower_angle_limit)
      , upper_angle_limit_(upper_angle_limit)
      , centering_offset_(centering_offset)
    {
    }

    /// Set angle betweeen -90 to 90
    void setAngle(int8_t angle)
    {
        Serial.print("Angle before: ");
        Serial.println(angle);
        angle = clamp<int8_t>(angle, lower_angle_limit_, upper_angle_limit_);

        Serial.print("Angle after: ");
        Serial.println(angle);
        // A servo usually takes a pulse 0-~2.5 ms that corresponds to
        // 0-180 degrees. For a 16 bit PWM at 50 Hz corresponds
        // to around 2.5 ms
        ledcWrite(channel_, map(angle + centering_offset_, -90, 90, 0, 8888));
    }

  private:
    const int8_t lower_angle_limit_;
    const int8_t upper_angle_limit_;
    const int8_t centering_offset_;
};

#endif // __SERVO_h