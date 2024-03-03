//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	WienerPlain.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <cmath>
#include <string>

#include "rv_library.h"
#include "lib_val.h"
#include "VecB.h"

#include "WienerPlain.h"

#include "IO_Manager.h"
#include "ParameterN.h"
#include "ParameterTmax.h"

#include "AppFactoryRegistration.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  traits
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<WienerPlain>()
{
    static std::string name = "WienerPlain";
    return name;
}

template<> std::string ClassID<WienerPlain>()
{
    static std::string name = "p";  // plain
    return name;
}

template<> std::string BaseClassName<WienerPlain>()
{
    return ClassName<WienerBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<WienerBase, WienerPlain> RegisterWienerPlain;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	onstructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

WienerPlain::WienerPlain()
:   N_(IO_Manager::Instance().GetValue<long>(ParameterN()))
,   z0_(0.0)
{
	double Tmax = IO_Manager::Instance().GetValue<double>(ParameterTmax());
    dt_ =  Tmax / N_;  						  // time step

    rt_ = std::sqrt(dt_);            		  // speed up
}

WienerPlain::~WienerPlain()
{}
 						 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FillUpPath()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void WienerPlain::FillUpPath(VecB & path)
{
    long lb = path.LBound();
    long ub = path.UBound();

    path[lb] = z0_;
        
    for(long i = lb + 1;  i <= ub; ++i)       //for each time step
    {
        path[i] = next_z(path[i-1]);
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FillUpSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void WienerPlain::FillUpSlice(const double T, VecB & slice) const
{
    long lb = slice.LBound();
    long ub = slice.UBound();
    
    double rT = std::sqrt(N_ * dt_);
    
    for(long i = lb;  i <= ub; ++i)     // uncorrelated random sample for time T
    {
	    double w = rv::GetNormalVariate();
       	slice[i] = w * rT;;
    }
}
                 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	next_z()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double WienerPlain::next_z(double z)
{
    double w = rv::GetNormalVariate();
    return z + rt_*w;  // adds a Wiener increment
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string WienerPlain::GetName() const {return ClassName<WienerPlain>();}
std::string WienerPlain::GetID() const {return ClassID<WienerPlain>();}
std::string WienerPlain::GetBaseClassName() const {return BaseClassName<WienerPlain>();}
std::string WienerPlain::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  z0 =  " + lv::DoubleToString(z0_) + '\n';
    nm += "  N =   " + lv::DoubleToString(N_) + '\n';
    nm += "  T =   " + lv::DoubleToString(N_*dt_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX







