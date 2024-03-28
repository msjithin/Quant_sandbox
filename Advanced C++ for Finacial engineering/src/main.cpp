//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Main function to drive the application
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <iostream>
#include "utility.h"
#include "helper.h"
#include "OptionMCutility.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  main()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

int main(int argc, char *argv[])
{
    
    // basic I/O
    double S_0  = 100;          // Initial stock price
    double r    = 0.05;         // Riskless rate
    double sig  = 0.20;         // volatility
    double X    = 100;          // Option strike 
    double T    = 1;            // Time to maturity

    long N      = 100;          // Time steps
    long M      = 10000;        // Sample paths

    double c;                   // option value
    double se;                  // Standard error
    double discount;            // discount 

    double st_time = clock();

    double dt = T/N;
    double drift = (r - 0.5*sig*sig)*dt;
    double sgrt = sig*std::sqrt(dt);

    double acc_vals = 0.0;
    double acc_squs = 0.0;

    for(long j = 1; j <= M; ++j)            // For each path
    {
        ut::OutputCounter(j, M, 5000);
        
        double path_S = S_0;                // Stock price at a point in the 
        
        for(long i = 1; i <= N; ++i)        // For each time steap
        {
            path_S = mc::next_S(path_S, drift, sgrt);
        }  
        
        double payoff = mc::ComputePayoff(path_S, X);
        
        acc_vals += payoff;
        acc_squs += payoff*payoff;       
    }
    
    discount = std::exp(-r*T);
        
    c = discount*acc_vals/M;
    se = discount*std::sqrt(acc_squs - acc_vals*acc_vals/M)/M;
    
    double el_time = (clock() - st_time)/CLOCKS_PER_SEC;
    
    ut::OutputLine("Option value ", c);
    ut::OutputLine("se           ", se);
    ut::OutputLine("time taken   ", el_time);
            

    return ut::PauseAndReturn();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

