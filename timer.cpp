#include "timer.h"

Timer::Timer(){};
Timer::~Timer(){};
void Timer::start()
{
    clock_gettime(CLOCK_MONOTONIC, &begin);
}
void Timer::stop()
{
    clock_gettime(CLOCK_MONOTONIC, &end);
}
void Timer::print_time()
{
    elapsed = end.tv_sec - begin.tv_sec;
    elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    cout<< double(elapsed)<<"\n";
}
double Timer::get_time(){
    elapsed = end.tv_sec - begin.tv_sec;
    elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    return elapsed;
}
