#include "stopwatch.h"

stopwatch::stopwatch(QWidget *parent):QDialog (parent)
{
    running = false;
}

stopwatch::~stopwatch()
{
    timer.stop();
}

//function returns current time on stopwatch
QString stopwatch::GetTime()
{
    int ms = time_on_stopwatch.msecsSinceStartOfDay() + QTime::currentTime().msecsSinceStartOfDay() - last_start.msecsSinceStartOfDay();

    return QTime::fromMSecsSinceStartOfDay(ms).toString("mm:ss:zzz");
}

//function starts or resumes stopwatch
void stopwatch::Start()
{
   if(!timer.isActive())
    {
        last_start = QTime::currentTime();
        timer.start(20);
    }
}
//function stops stopwatch
void stopwatch::Stop()
{
    if(timer.isActive())
    {
        timer.stop();
        time_on_stopwatch = QTime::fromMSecsSinceStartOfDay(time_on_stopwatch.msecsSinceStartOfDay()+QTime::currentTime().msecsSinceStartOfDay()- last_start.msecsSinceStartOfDay());
    }
}

//function resets stopwatch
void stopwatch::Reset()
{
    timer.stop();
    time_on_stopwatch = QTime::fromMSecsSinceStartOfDay(0);
}
