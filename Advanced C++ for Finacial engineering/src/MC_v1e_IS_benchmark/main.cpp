//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  MC_v1e_IS_benchmark
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "rv_library.h"
#include "utility.h"
      
#include <algorithm>
#include <cmath>
#include <ctime>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  declarations
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double weight(double mu, double sig, double x);

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  main()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

int main(int argc, char *argv[])
{
    ut::OutputLine("Monte Carlo version 1e:  benchmarks IS against a normal distribution");
    
    long N = 100;              // number of replications
    long M = 10000;         // number of samples per replication
    double q = 0.99;           // 99th quantile
    double qc = 1 - q;         // proportion above the quantile q
    double nq = rv::cndev(q);  // inverse of normal distribution function
    
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  main loop:  plain
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    ut::OutputLine("\nDoing plain\n");

    double st_time = clock();
    double acc_squ = 0.0;
    double plain_v;
    
    for(long i = 1; i <= N; ++i)
    {
        double acc_p = 0.0;
        //double acc_s = 0.0;
        
        for(long j = 1; j <= M; ++j)
        {
            //ut::OutputCounter(j, M, 50000);
            
            double x = rv::GetNormalVariate();
            double p = (x >= nq) ? 1.0 : 0.0;
            
            acc_p += p;
            //acc_s += p*p;
        }
    
        plain_v = acc_p/M;
        //double plain_se = std::sqrt(acc_s - acc_p*acc_p / M) / M;
        acc_squ += (plain_v - qc)*(plain_v - qc);
    }
        
    double plain_se = std::sqrt(acc_squ)/N;
    double plain_time = (clock() - st_time)/CLOCKS_PER_SEC;  // time taken to do plain MC

    ut::OutputLine("\nPlain value        ", plain_v);
    ut::OutputLine("Plain se           ", plain_se);
    ut::OutputLine("time taken,  plain ", plain_time);

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  main loop:  IS
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    ut::OutputLine("\nDoing IS\n");
    
    double mu = 1;   // parameters of the IS distribution
    double sig = 2;
    
    st_time = clock();
    acc_squ = 0.0;
    double IS_v;
    
    for(long i = 1; i <= N; ++i)
    {    
        double acc_p = 0.0;
        //double acc_s = 0.0;
        
        for(long j = 1; j <= M; ++j)
        {
            //ut::OutputCounter(j, M, 50000);
            
            double x = sig*rv::GetNormalVariate() + mu;
            double p = (x >= nq) ? weight(mu, sig, x) : 0.0;
            
            acc_p += p;
            //acc_s += p*p;
        }
    
        IS_v = acc_p/M;
        //double IS_se = std::sqrt(acc_s - acc_p*acc_p / M) / M;
        acc_squ += (IS_v - qc)*(IS_v - qc);
    }
    
    double IS_se = std::sqrt(acc_squ)/N;
    double IS_time = (clock() - st_time)/CLOCKS_PER_SEC;  // time taken to do plain MC

    ut::OutputLine("\nIS value           ", IS_v);
    ut::OutputLine("IS se              ", IS_se);
    ut::OutputLine("time taken,  IS    ", IS_time);

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //  end stuff
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    
    double speed_up = (plain_se*plain_se*plain_time)/(IS_se*IS_se*IS_time);
    ut::OutputLine("\nspeed_up           ", speed_up);
            
    return ut::PauseAndReturn();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  weight()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double weight(double mu, double sig, double x)
{
    double sx = (x - mu)/sig;
    return sig*std::exp(-0.5*(x*x - sx*sx));
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX





