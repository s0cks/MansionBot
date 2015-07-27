#ifndef MANSIONBOT_TIMER_H
#define MANSIONBOT_TIMER_H

#include <time.h>

class Timer{
private:
    time_t begin;
public:
    Timer(){}
    ~Timer(){}

    void
    start(){
        this->begin = time(0);
    }

    double
    elapsed(){
        return difftime(time(0), this->begin);
    }
};

#endif