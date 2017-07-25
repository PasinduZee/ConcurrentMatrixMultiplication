#include <stdio.h>
#include <unistd.h>			//sleep    
#include <cmath>			//pows
#include <time.h> 			//seed_timers	
#include <iostream>
#include <stdint-gcc.h>     //int_fast16_t
#include <omp.h>
#include "timer.h"
#include "auxilary.h"

using namespace std;

//-------------Class Test------------------//
/*
 * This class includes main multiplication code with minimum required sample count calculation functions
 * */
class Test
{
private:
    double mean;
    double s;
public:	
	Test();
    ~Test();

	void run_n(int n,bool sample_count_correct);
	void run_sample_count(int sample_count,int n,int version);
	double run_single_test(int n,int version);
	int correct_sample_count(double mean,double s,double z,double r);
    void transpose_matrix(double** matB,double** matA,int n);
    void serial_multiply(double** matC,double** matA,double** matB,int n);
	void parallel_multiply(double** matC,double** matA,double** matB,int n);
    void optimized_multiply(double** matC,double** matA,double** matB,double** matB_transposed,int n);
};

Test::Test() {};
Test::~Test() {};

/*
 *	time_taken[0] : serial
 *	time_taken[1] : parallel
 *	time_taken[2] : optimized
 *	run all the versions (serial,parallel and optimized for a given n)
 *	first it will run for 10 iterations.
 *	then it calculates the minimum required count using mean,s ,z=1.96 and r=5
 *	if sample_count_correct flag is true & 10 is not enough we rerun the test.
 */
void Test::run_n(int n,bool sample_count_correct)
{
	mean=0;
	s=0;
	double * time_taken = new double[3];
	int minimum_required_sample_count;
	for (int version=0 ; version <3;version++)
	{

		run_sample_count(10,n,version);
		minimum_required_sample_count=correct_sample_count(mean,s,1.960000,5);
		if(minimum_required_sample_count>10 && sample_count_correct)
		{
			//re run if 10 is not enough as the sample count
			run_sample_count(minimum_required_sample_count+1,n,version);
		}
		time_taken[version]=mean;
	}
	double parallel_speedup=time_taken[0]/time_taken[1];
	double optimized_speedup=time_taken[0]/time_taken[2];
	cout<<n<<"\n";
	cout<<"serial:"<< time_taken[0]<<"\n";
	cout<<"parallel:"<< time_taken[1]<<"\n";
	cout<<"optimized:"<< time_taken[2]<<"\n";
	cout<<"parallel_speedup:"<<parallel_speedup<<"	"<<"optimized_speedup:"<<optimized_speedup<<"   "<<"Diff:"<<optimized_speedup-parallel_speedup<<"\n\n";
	delete time_taken;


}

/*
 *	version=0 : serial
 *	version=1 : parallel
 *	version=2 : optimized
 */
double Test::run_single_test(int n,int version)
{
	//seed and required object creation
	srand(time(NULL));
	Auxilary* Aux=new Auxilary();
	Timer t;
	
	//Empty matrix generation
	double** matA=	Aux->generate_matrix(n);
	double** matB=	Aux->generate_matrix(n);
	double** matC=	Aux->generate_matrix(n);
	double** matB_transposed=Aux->generate_matrix(n);
	
	//Randomize matA and B and get transpose
	Aux->randomize_matrix(matA,n);
	Aux->randomize_matrix(matB,n);

	if(version==0)
	{
		t.start();
		serial_multiply(matC,matA,matB,n);
		t.stop();
	}
	else if(version==1)
	{
		t.start();
		parallel_multiply(matC,matA,matB,n);
		t.stop();
	}
	else if(version==2)
	{
		t.start();
		//we feed transposed B to increase caching advantage
		optimized_multiply(matC,matA,matB,matB_transposed,n);
		t.stop();
	}

	//clean memory to stop memory leaks
	Aux->delete_matrix(matA,n);
	Aux->delete_matrix(matB,n);
	Aux->delete_matrix(matC,n);
	Aux->delete_matrix(matB_transposed,n);
	
	//return time for a a single run
	return t.get_time();
}

void Test::run_sample_count(int sample_count, int n,int version)
{
    double* test_results=new double[sample_count];
    mean=0;
	s=0;
    for (int i=0;i<sample_count;i++)
    {
        double value=run_single_test(n,version);
        mean+=value;
        test_results[i]=value;
		sleep(2);
	}
    mean=mean/sample_count;
    for (int i=0;i<sample_count;i++)
    {
        s+=pow(mean-test_results[i],2);
    }
    s=double(s)/sample_count;
    s = sqrt(s);
    delete test_results;

}

int Test::correct_sample_count(double mean, double s, double z, double r)
{
    int sample_c =ceil(pow(double(100*z*s)/(r*mean),2));
    return  sample_c;
}

void Test::transpose_matrix(double** matB,double** matA,int n)
{
    #pragma omp parallel for
    for (int_fast16_t i = 0; i < n; i++)
    {
        for (int_fast16_t j = 0; j < n; j++)
        {
            matB[i][j]=matA[j][i];
        }
    }
}
void Test::serial_multiply(double** matC,double** matA,double** matB,int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			double sum=0;
			for (int k = 0; k < n; k++)
			{
				sum += (matA[i][k]) * (matB[k][j]);
			}
			matC[i][j]=sum;
		}
	}
}
void Test::parallel_multiply(double** matC,double** matA,double** matB,int n)
{
	#pragma omp parallel for
	for (int i = 0; i < n; i++)
	{
		#pragma omp parallel for
		for (int j = 0; j < n; j++)
		{
			double sum=0;
			for (int k = 0; k < n; k++)
			{
				sum += (matA[i][k]) * (matB[k][j]);
			}
			matC[i][j]=sum;
		}
	}
}
void Test::optimized_multiply(double** matC,double** matA,double** matB,double** matB_transposed,int n)
{
	//Get transpose
    transpose_matrix(matB_transposed,matB,n);

    #pragma omp parallel for 
    for (int_fast16_t i = 0; i < n; i++)
	{
		for (int_fast16_t j = 0; j < n; j++)
		{
			double sum=0;
			for (int_fast16_t k= 0; k < n; k++)
			{
				sum += (matA[i][k]) * (matB_transposed[j][k]);
			}
			matC[i][j]=sum;
		}
	}
}
//--------------End of Class Test-----------//

int main(int argc, char ** argv) 
{
    cout<<"Note: Program use sleep to get good randomization using seed\n\n";
	Test* test=new Test();

	for (int n=200;n<2001;n+=200)
	{
		test->run_n(n,false);
	}

	delete test;
	return 0;
	
}
