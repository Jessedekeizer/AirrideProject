#include "Timer.h"
#include <Arduino.h>

Timer::Timer(float durationSeconds, TimerCallback callbackFunc, bool repeat, int repeatCount)
    : duration(durationSeconds * 1000),
      callback(callbackFunc),
      repeating(repeat),
      remainingRepeats(repeatCount),
      finished(false)
{
    startTime = millis();
}

bool Timer::Update()
{
    if (!finished && millis() - startTime >= duration)
    {
        if (callback)
        {
            callback();
        }
        if (repeating)
        {
            if (remainingRepeats > 0)
            {
                remainingRepeats--;
                if (remainingRepeats == 0)
                {
                    finished = true;
                    return true;
                }
            }

            Reset();
        }
        else
        {
            finished = true;
            return true; // tell manager to remove
        }
    }
    return false; // not ready yet
}

bool Timer::IsFinished() const
{
    return finished;
}

void Timer::Reset()
{
    startTime = millis();
    finished = false;
}

unsigned long Timer::GetDuration() const
{
    return duration;
}
// Timer.cpp