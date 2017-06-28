g++  serial.cpp -o serial timer.cpp auxilary.cpp
g++  -fopenmp par_for.cpp -o par_for timer.cpp auxilary.cpp
g++  -fopenmp optimized.cpp -o optimized timer.cpp auxilary.cpp

