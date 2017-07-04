#ifdef USE_RESTRICT
#endif

#include <stdio.h>
#include <unistd.h>			//sleep    
#include <cmath>			//pows
#include <time.h> 			//seed_timers	
#include "auxilary.h"
#include "timer.h"

#define N 1000

using namespace std;

//global variable
int * sample_count_per_n;

//-------------Class Test------------------//
class Test
{
private:
    double mean;
    double s;
public:	
	Test();
    ~Test();
	double run_single_test(int n);
    void aggregate_results(int sample_count,int n);
	int correct_sample_count(double mean,double s,double z,double r);
	void multiply(double** matC,double** matA,double** matB,int n);
	void print(double* __restrict__ matA,int n);
};

Test::Test() {};
Test::~Test() {};

double Test::run_single_test(int n)
{
	//seed and required class defenitions
	Auxilary* Aux=new Auxilary();
	Timer t;
	
	//Empty matrix generation
	double* __restrict__ matA =new double[N*N];
	double* __restrict__ matB =new double[N*N];
	double* matC =new double[N*N];
	double* __restrict__ matB_transposed =new double[N*N];

	
	//populate matA, matB and matB_transposed
	srand(time(NULL));		
	for (int i=0;i<N;i++)
	{
		for (int j=0;j<N;j++)
		{
			double b=Aux->fRand();
			matA[i*N+j]=b;
			double c=Aux->fRand();
			matB[i*N+j]=c;
			matB_transposed[j*N+i]=c;
		}	
	}

	//start timer and run multiply 
	t.start();		

	#pragma omp parallel for
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			double sum=0;
			for (int k = 0; k < N; k++)
			{
				sum += (matA[i*N+k]) * (matB_transposed[j*N+k]);
			}
			matC[i*N+j]=sum;
		}
	}

	t.stop();

	//cout check
	print(matA,n);
	print(matB,n);
	print(matC,n);

	//clean memory to stop memory leaks
	delete matA;
	delete matB;
	delete matC;
	delete matB_transposed;
	delete Aux;
	Aux=NULL;matA=NULL;matB=NULL;matC=NULL;matB_transposed=NULL;


	//return time for a asingle run
	return t.get_time();
}

void Test::aggregate_results(int sample_count,int n)
{
    double* test_results=new double[sample_count];
    mean=0;
    for (int i=0;i<sample_count;i++)
    {
        double value=run_single_test(n);
        mean+=value;
        test_results[i]=value;
		sleep(2);
	}
    mean=mean/sample_count;
    s=0;
    for (int i=0;i<sample_count;i++)
    {
        s+=pow(mean-test_results[i],2);
    }
    s=double(s)/sample_count;
    s = sqrt(s);
    delete test_results;
    cout<<	"n : "<<n<<"\n";
    cout<<	"sample count : "<<sample_count<<"\n";
	cout<<	"mean <sec>:"<<mean<<"\n";
    cout << "s <sec>:"<<s<<"\n\n";
    
    //update the needed sample count
    sample_count_per_n[n/200]=correct_sample_count(mean,s,1.960000,5);
}

int Test::correct_sample_count(double mean, double s, double z, double r)
{
    int sample_c =ceil(pow(double(100*z*s)/(r*mean),2));
    return  sample_c;
}

void Test::multiply(double** matC,double** matA,double** matB,int n)
{
	#pragma omp parallel for
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			double sum=0;
			for (int k = 0; k < n; k++)
			{
				sum += (matA[i][k]) * (matB[j][k]);
			}
			matC[i][j]=sum;
		}
	}
}

void Test::print(double* __restrict__ matA,int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout<<matA[i*n+j]<<" ";
		}
		cout<<"\n";
	}
	cout <<"\n";
}

//--------------End of Class Test-----------//

int main(int argc, char ** argv) 
{
	
	Test* test=new Test();
	//Store sufficient sample count for each n .it is calculated from the first round
	sample_count_per_n=new int [11];
	
	cout<<"Optimized Version (transopnse + single parfor) Results"<<"\n\n";
	cout<<"-----------First Round - Calculate needed sample counts--------"<<"\n\n";
	//First Round
	//for (int i=200;i<2001;i+=200)
	//{
		/*
			Set the initial sample count to 10.
			The mean and s is calculated and the required sample count is
			stored to the sample_count_per_n array;
		*/
		test->aggregate_results(1,4);
	//}

/*
	cout<<"Required Sample Counts"<<"\n";
	for(int i=1;i<11;i++)
	{
		cout<<sample_count_per_n[i]<<" "; 
	}
*/
	cout<<"\n";
	cout<<"-----------Second Round - run test for required iterations--------"<<"\n\n";
	
	//Second Round
//	for (int i=200;i<2001;i+=200)
//	{
		/*
			From the first round minimum required sample count is set
			Testing is re done
		*/
//		test->aggregate_results(sample_count_per_n[i/200],i);
//	}

	delete test;
	return 0;
	
}
