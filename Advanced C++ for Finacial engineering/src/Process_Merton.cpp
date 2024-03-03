//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Process_Merton.cpp.  dSt = µStdt + sigStdWt + (eta-1)dq,  intensity lambda
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <cmath>
#include <stdexcept>
#include "VecB.h"
#include "lib_val.h"

#include "rv_library.h"

#include "Process_Merton.h"

#include "IO_Manager.h"
#include "AppFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterS0.h"
#include "ParameterSigma.h"
#include "ParameterR.h"
#include "ParameterTmax.h"
#include "ParameterN.h"
#include "ParameterLambda.h"
#include "ParameterEta.h"
#include "ParameterNu.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<Process_Merton>()
{
    return "Process_Merton";
}

template<> std::string ClassID<Process_Merton>()
{
    return "m";
}

template<> std::string BaseClassName<Process_Merton>()
{
    return ClassName<ProcessBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<ProcessBase, Process_Merton> RegisterProcess_Merton;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Process_Merton::Process_Merton()
:   S_0_(IO_Manager::Instance().GetValue<double>(ParameterS0()))
,   sig_(IO_Manager::Instance().GetValue<double>(ParameterSigma()))
,   r_(IO_Manager::Instance().GetValue<double>(ParameterR()))
,   lambda_(IO_Manager::Instance().GetValue<double>(ParameterLambda()))		// jumpt rate
,   eta_(IO_Manager::Instance().GetValue<double>(ParameterEta()))			// log jump mean
,   nu_(IO_Manager::Instance().GetValue<double>(ParameterNu()))				// log jump sd
{
    long N = IO_Manager::Instance().GetValue<long>(ParameterN());
    double Tmax = IO_Manager::Instance().GetValue<double>(ParameterTmax());
	dt_ = Tmax/N;

    drift_ = r_ - lambda_*std::exp(eta_ + 0.5*nu_*nu_) - 0.5*sig_*sig_;
    sgdt_ = sig_*sig_*dt_;
}
                

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Rollback stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Process_Merton::GetDtDiscount(double t) const {return std::exp(-r_*t);}
double Process_Merton::GetSScale(double t) const {return std::exp(-drift_*t);}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	MC stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Process_Merton::Next_S(double S) const
{
    double w = rv::GetNormalVariate();

    long n = rv::Poisson_jumps2(lambda_*dt_);
    double jump_var = nu_*nu_*n;
    
    double tot_sd = std::sqrt(sgdt_ + jump_var);

    return S*std::exp(drift_*dt_ + eta_*n + tot_sd*w);
}

void Process_Merton::FillUpPath(std::vector<double> & path) const
{
    long N = path.size() - 1;
    
    path[0] = S_0_;
    
    for(long i = 1; i <= N; ++i)
    {
        path[i] = Next_S(path[i-1]);
    }       
}
 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FillUpSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Process_Merton::FillUpSlice(const double T, VecB & S, const VecB & Z) const
{
    long lb = Z.LBound();
    long ub = Z.UBound();

    for(long i = lb;  i <= ub; ++i)       //for each time step
    {
        S[i] = ExplicitSoln(S_0_, T, Z[i]);
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ExplicitSoln().  z is a Wiener increment over the period T
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Process_Merton::ExplicitSoln(const double T, const double z) const
{
    return ExplicitSoln(S_0_, T, z);
}

double Process_Merton::ExplicitSoln(const double S, const double T, const double z) const
{
	throw std::runtime_error("Process_Merton::ExplicitSoln:  Not implemented");
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string Process_Merton::GetName() const {return ClassName<Process_Merton>();}
std::string Process_Merton::GetID() const {return ClassID<Process_Merton>();}
std::string Process_Merton::GetBaseClassName() const {return BaseClassName<Process_Merton>();}
std::string Process_Merton::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  r =   " + lv::DoubleToString(r_) + '\n';
    nm += "  sig = " + lv::DoubleToString(sig_) + '\n';
    nm += "  S0 =  " + lv::DoubleToString(S_0_) + '\n';
    nm += "  lambda =  " + lv::DoubleToString(lambda_) + '\n';
    nm += "  eta =  " + lv::DoubleToString(eta_) + '\n';
    nm += "  nu =  " + lv::DoubleToString(nu_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
