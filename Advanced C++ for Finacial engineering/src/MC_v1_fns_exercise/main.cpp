//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  MC_v1_fns_exercise
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "rv_library.h"
#include "utility.h"

#include <algorithm>
#include <cmath>
#include <ctime>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  struct
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

struct MC_data
{
    double S_0;
    double r;
    double sig;
    
    double X;
    double T;
    char o_type;
    
    long M;     // number of sample paths
    long N;     // number of time steps
};
      
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  prototypes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ReadInData(MC_data & dat);

double Next_S(double S, double drift, double sgrt);

double ComputePayoff(double S, double sum, double X, char p_type);
double ComputeCallPayoff(double S, double X);
double ComputePutPayoff(double S, double X);

void Accumulate(double val, double & acc_vals, double & acc_squs);
void ComputeValues(MC_data dat, double acc_vals, double acc_squs,  double discount, double & c, double & se);

void DoOutput(double c, double se, double el_time);

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  main()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

int main(int argc, char *argv[])
{
    ut::OutputLine("Monte Carlo version 1:  uses functions,  exercise");
    
    MC_data dat;
    ReadInData(dat);
    
    double st_time = clock();
    
	double acc_vals = 0;
	double acc_squs = 0;

    double dt = dat.T/dat.N;
    double drift = (dat.r - 0.5*dat.sig*dat.sig)*dt;
    double sgrt = dat.sig*std::sqrt(dt);
    
    for(long j = 1; j <= dat.M; ++j)
    {
        ut::OutputCounter(j, dat.M, 50000);
        
        double S = dat.S_0;
        double sum = dat.S_0;
         
        for(long i = 1; i <= dat.N; ++i)
        {
            S = Next_S(S, drift, sgrt);
            
            sum += S;  // always computes this.  overhead.
        }

		sum /= dat.N;  // the path average
                
        double payoff = ComputePayoff(S, sum, dat.X, dat.o_type);  

		Accumulate(payoff,  acc_vals,  acc_squs);
    }

    double discount = std::exp(-dat.r*dat.T);
	double c;  // option value
	double se; // option se
	
	ComputeValues(dat, acc_vals, acc_squs,  discount,  c, se);
    
    double el_time = (clock() - st_time)/CLOCKS_PER_SEC;
    
    DoOutput(c, se, el_time);
            
    return ut::PauseAndReturn();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  ReadInData
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ReadInData(MC_data & dat)
{
    dat.S_0 = 100;
    dat.r = 0.05;
    dat.sig = 0.20;
    
    dat.X = 100;
    dat.T = 1;
    dat.o_type = 'b';	// c for plain call,  p for plain put
    					// a for average rate call,  b for average rate put
        
    dat.M = 100000;  // number of sample paths
    dat.N = 100;     // number of time steps
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Generating function
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Next_S(double S, double drift, double sgrt)
{
    double w = rv::GetNormalVariate();
    return S*std::exp(drift + sgrt*w);       
}
       
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Payoff functions
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ComputePayoff(double S, double sum, double X, char o_type)
{
    double payoff;
    
    switch(o_type)
    {
        case 'p': payoff = ComputePutPayoff(S, X);
                  break;  
                  
        case 'c': payoff = ComputeCallPayoff(S, X);
                  break; 
                  
        case 'a': payoff = ComputeCallPayoff(sum, X);
                  break;  
                  
        case 'b': payoff = ComputePutPayoff(sum, X);
                  break; 
                  
        default:  // really must validate o_type so that only valid values allowed
                  payoff = -1000;  // arbitrary value.  should throw
				  break;
    }
    return payoff;
}

double ComputeCallPayoff(double S, double X)
{
    return std::max(0.0, S - X);
}

double ComputePutPayoff(double S, double X)
{
    return std::max(0.0, X - S);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Accumulate()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Accumulate(double val, double & acc_vals, double & acc_squs)
{
    acc_vals += val;
    acc_squs += val*val;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  ComputeValues()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ComputeValues(MC_data dat, double acc_vals, double acc_squs,  double discount, double & c, double & se)
{
    c = discount*acc_vals/dat.M;
    se = discount*std::sqrt(acc_squs - acc_vals*acc_vals/dat.M)/dat.M;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Output function
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void DoOutput(double c, double se, double el_time)
{
    ut::OutputLine("Option value ", c);
    ut::OutputLine("se           ", se);
    ut::OutputLine("time taken   ", el_time);
}
 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX





