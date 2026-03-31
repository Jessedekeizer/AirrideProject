#ifndef TIMER_H
#define TIMER_H

#include <functional>

typedef std::function<void()> TimerCallback;

class Timer
{
public:
    Timer(float durationSeconds, TimerCallback callbackFunc, bool repeat = false, int repeatCount = -1);

    bool Update();
    bool IsFinished() const;
    void Reset();
    unsigned long GetDuration() const;

private:
    unsigned long startTime;
    unsigned long duration;
    TimerCallback callback;
    bool finished;
    bool repeating;
    int remainingRepeats; // -1 means infinite
};

#endif
// Timer.h