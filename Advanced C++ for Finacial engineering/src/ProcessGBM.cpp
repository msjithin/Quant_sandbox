//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ProcessGBM.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <cmath>
#include <vector>
#include "VecB.h"
#include "lib_val.h"

#include "rv_library.h"

#include "ProcessGBM.h"
#include "WienerBase.h"

#include "IO_Manager.h"
#include "AppFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterS0.h"
#include "ParameterSigma.h"
#include "ParameterR.h"
#include "ParameterTmax.h"
#include "ParameterN.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<ProcessGBM>()
{
    return "ProcessGBM";
}

template<> std::string ClassID<ProcessGBM>()
{
    return "g";
}

template<> std::string BaseClassName<ProcessGBM>()
{
    return ClassName<ProcessBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<ProcessBase, ProcessGBM> RegisterProcessGBM;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

ProcessGBM::ProcessGBM()
:   S_0_(IO_Manager::Instance().GetValue<double>(ParameterS0()))
,   sig_(IO_Manager::Instance().GetValue<double>(ParameterSigma()))
,   r_(IO_Manager::Instance().GetValue<double>(ParameterR()))
{
    wie_ = AppFactory<WienerBase>::Instance().CreateObject();   	// Owns

    long N = IO_Manager::Instance().GetValue<long>(ParameterN());
    double Tmax = IO_Manager::Instance().GetValue<double>(ParameterTmax());
	dt_ = Tmax/N;

    drift_ = r_ - 0.5*sig_*sig_;
    rt_ = std::sqrt(dt_);
}

ProcessGBM::~ProcessGBM()
{
	delete wie_;
}
                
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Rollback stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessGBM::GetDtDiscount(double t) const {return std::exp(-r_*t);}
double ProcessGBM::GetSScale(double t) const {return std::exp(-drift_*t);}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	MC stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double ProcessGBM::Next_S(double S) const
{
    double w = rv::GetNormalVariate();
    return ExplicitSoln(S, dt_, rt_*w);  // rt_*w is a Wiener increment
}

void ProcessGBM::FillUpPath(std::vector<double> & Spath) const
{
    long N = Spath.size() - 1;;

    VecB Wpath(0, N);   
    wie_->FillUpPath(Wpath);
    
    Spath[0] = S_0_;

    for(long i = 1;  i <= N; ++i)       // for each time step
    {
        Spath[i] = ExplicitSoln(Spath[i-1], dt_, Wpath[i] - Wpath[i-1]);
    }
}
 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FillUpSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ProcessGBM::FillUpSlice(const double T, VecB & S, const VecB & Z) const
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

double ProcessGBM::ExplicitSoln(const double T, const double z) const
{
    return ExplicitSoln(S_0_, T, z);
}

double ProcessGBM::ExplicitSoln(const double S, const double T, const double z) const
{
    return S * std::exp(drift_ * T + sig_*z);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string ProcessGBM::GetName() const {return ClassName<ProcessGBM>();}
std::string ProcessGBM::GetID() const {return ClassID<ProcessGBM>();}
std::string ProcessGBM::GetBaseClassName() const {return BaseClassName<ProcessGBM>();}
std::string ProcessGBM::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  r =   " + lv::DoubleToString(r_) + '\n';
    nm += "  sig = " + lv::DoubleToString(sig_) + '\n';
    nm += "  S0 =  " + lv::DoubleToString(S_0_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
