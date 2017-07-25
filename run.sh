g++ main.cpp -fopenmp -O3 -march=native -mtune=native -fmessage-length=0 -funroll-loops -ftree-vectorize -ftree-vectorizer-verbose=1  -ffast-math -fassociative-math -o concurrent_mat_mul auxilary.cpp timer.cpp

./concurrent_mat_mul

