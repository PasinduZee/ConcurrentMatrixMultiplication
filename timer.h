#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <time.h> 
#include <iostream>

using namespace std;

class Timer {
private:
    struct timespec begin, end;
    double elapsed;
public:
    Timer();
    ~Timer();
    void start();
    void stop();
    void print_time();
    double get_time();
};
#endif 