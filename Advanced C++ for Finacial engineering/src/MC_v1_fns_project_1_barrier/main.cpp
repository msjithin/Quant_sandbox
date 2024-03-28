//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  MC_v1_fns_project_1_barrier
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
    double K;   	// barrier level
    double T;
    
    char o_type;    // p for plain,  a for average rate, (option type)
					// l for lookback, b for barrier
    char p_type;    // c for call,  p for put  (payoff type)
    char d_type;    // u for up,  d for down  (direction type)
    char k_type;    // i for in,  o for out   (knock type)

    long M;     	// number of sample paths
    long N;     	// number of time steps
};
      
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  prototypes
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ReadInData(MC_data & dat);
bool Validate(MC_data & dat);

double Next_S(double S, double drift, double sgrt);

double ComputePayoff(MC_data dat, double S, bool above, bool below, double av, double min_S, double max_S);

double ComputeLookbackPayoff(double S, double min_S, double max_S, char p_type);
double ComputeBarrierPayoff(double S, double X, bool above, bool below, char p_type, char d_type, char k_type);
double ComputePayoff(double S, double X, char p_type);

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
    ut::OutputLine("Monte Carlo version 1:  uses functions,  project,  1,  barrier options,  etc");
    
    MC_data dat;
    ReadInData(dat);
    
	if (!Validate(dat)) return ut::PauseAndReturn();

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
        double min_S = dat.S_0;
        double max_S = dat.S_0;
        
        bool above = (S >= dat.K);  // true if always above
    	bool below = (S <= dat.K);  // true if always below

        for(long i = 1; i <= dat.N; ++i)
        {
            S = Next_S(S, drift, sgrt);
            
            sum += S;
            
 			min_S = std::min(min_S, S);
			max_S = std::max(max_S, S);
			
			above = above && (S >= dat.K);
			below = below && (S <= dat.K);
       }

		double av = sum / dat.N;
                
        double payoff = ComputePayoff(dat, S, above, below, av, min_S, max_S);

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
   	dat.K = 120;
    dat.T = 1;

	// the following values must be validated!!!
	dat.o_type = 'b';    // p for plain,  a for average rate, (option type)
						 // l for lookback, b for barrier
    dat.p_type = 'c';    // c for call,  p for put  (payoff type)
    dat.d_type = 'u';    // u for up,  d for down  (direction type)
    dat.k_type = 'i';    // i for in,  o for out   (knock type)

    dat.M = 100000;  // number of sample paths
    dat.N = 100;     // number of time steps
}

bool Validate(MC_data & dat)
{
	if (dat.o_type !='p' && dat.o_type !='a' && dat.o_type !='l' && dat.o_type !='b')
    {
		ut::OutputLine("Bad option type character", dat.o_type);
		return false;
	}

	if (dat.d_type !='u' && dat.d_type !='d')
    {
		ut::OutputLine("Bad direction type character", dat.d_type);
		return false;
	}

	if (dat.p_type !='p' && dat.p_type !='c')
    {
		ut::OutputLine("Bad payoff type character", dat.p_type);
		return false;
	}

	if (dat.k_type !='i' && dat.p_type !='o')
    {
		ut::OutputLine("Bad knock type character", dat.k_type);
		return false;
	}

	return true;
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
//  Payoff functions.  Excruciating set of switches....
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ComputePayoff(MC_data dat, double S, bool above, bool below, double av, double min_S, double max_S)
{
    double payoff;
    
    switch(dat.o_type)
    {
        case 'p': payoff = ComputePayoff(S, dat.X, dat.p_type);
                  break;  
                  
        case 'a': payoff = ComputePayoff(av, dat.X, dat.p_type);
                  break;

        case 'l': payoff = ComputeLookbackPayoff(S, min_S, max_S, dat.p_type);
                  break;

        case 'b': payoff = ComputeBarrierPayoff(S, dat.X, above, below, dat.p_type, dat.d_type, dat.k_type);
                  break;
    }
    return payoff;
}

double ComputeLookbackPayoff(double S, double min_S, double max_S, char p_type)
{
    double payoff;

    switch(p_type)
    {
        case 'p': payoff = ComputePutPayoff(S, max_S);
                  break;

        case 'c': payoff = ComputeCallPayoff(S, min_S);
                  break;
    }
    return payoff;
}

double ComputeBarrierPayoff(double S, double X, bool above, bool below, char p_type, char d_type, char k_type)
{
    double payoff = 0.0;

    switch(k_type)
    {
        case 'i': 	switch(d_type)
				    {
				        case 'u': 	if (!below) switch(p_type)  // if not always below the barrier
								    {
								        case 'p': payoff = ComputePutPayoff(S, X);
								                  break;

								        case 'c': payoff = ComputeCallPayoff(S, X);
								                  break;
								    }
				                  	break;

				        case 'd':	if (!above) switch(p_type)  // if not always above the barrier
								    {
								        case 'p': payoff = ComputePutPayoff(S, X);
								                  break;

								        case 'c': payoff = ComputeCallPayoff(S, X);
								                  break;
								    }
				                  	break;
				    }
                  	break;

        case 'o': 	switch(d_type)
				    {
				        case 'u':	if (below) switch(p_type)  // if always below the barrier
								    {
								        case 'p': payoff = ComputePutPayoff(S, X);
								                  break;

								        case 'c': payoff = ComputeCallPayoff(S, X);
								                  break;
								    }
				                  	break;

				        case 'd': 	if (above) switch(p_type)  // if always above the barrier
								    {
								        case 'p': payoff = ComputePutPayoff(S, X);
								                  break;

								        case 'c': payoff = ComputeCallPayoff(S, X);
								                  break;
								    }
				                  	break;
				    }
                  	break;
    }
    
    return payoff;
}

double ComputePayoff(double S, double X, char p_type)
{
	double payoff;
	
    switch(p_type)
    {
        case 'p': payoff = ComputePutPayoff(S, X);
                  break;

        case 'c': payoff = ComputeCallPayoff(S, X);
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





