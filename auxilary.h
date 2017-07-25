#pragma once
#ifndef AUXILARY_H
#define AUXILARY_H

#include <string>
#include <iostream>
#include <stdlib.h> 
#include <limits>
#include <omp.h>

/*
 * Auxilary  class which includes all support functions eg: random number generation,create matrix etc
 *
 */
using namespace std;

class Auxilary {
public:
    Auxilary();
    ~Auxilary();
   	double fRand();
	double** generate_matrix(int n);
	void delete_matrix(double** matA,int n);
	void randomize_matrix(double** matrix,int n);
    bool* is_equal(double** matA,double** matB,int n);
	void print(double** matA,int n);
	void transpose_matrix(double** matB,double** matA,int n);
	
};

#endif