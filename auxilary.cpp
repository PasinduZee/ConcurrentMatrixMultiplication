#include "auxilary.h"


Auxilary::Auxilary(){};
Auxilary::~Auxilary(){};

double** Auxilary::generate_matrix(int n)
{
	double** matrix;
	matrix = new double *[n]; 
	for (int i = 0; i < n; i++)
	{
		matrix[i] = new double[n];
	}
	return matrix;
	
}

void Auxilary::delete_matrix(double** matA,int n)
{
	for (int i = 0; i < n; i++)
	{
		delete matA[i];
	}
}
void Auxilary::randomize_matrix(double** matrix,int n)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{	
			double v = fRand();
			matrix[i][j]=v;
		}		
	}
	
}
double Auxilary::fRand()
{
    double val=std::numeric_limits<float>::max();
	double fMin=-val; double fMax=val;
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void Auxilary::print(double** matA,int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout<<matA[i][j]<<" ";
		}
		cout<<"\n";
	}
}



bool* Auxilary::is_equal(double** matA,double** matB,int n)
{
    bool* is_equal=new bool();
    *is_equal=true;
    for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if(matA[i][j]==matB[i][j]){}
            else
            {
                *is_equal=false;
                return is_equal;
            }
		}
	}
    return is_equal;
}
	