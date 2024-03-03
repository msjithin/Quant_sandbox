//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	WienerAntithetic.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <cmath>
#include <string>
#include <stdexcept>

#include "rv_library.h"
#include "lib_val.h"
#include "VecB.h"

#include "WienerAntithetic.h"
#include "WienerPlain.h"

#include "IO_Manager.h"
#include "ParameterN.h"
#include "ParameterTmax.h"

#include "AppFactoryRegistration.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  traits
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<WienerAntithetic>()
{
    static std::string name = "WienerAntithetic";
    return name;
}

template<> std::string ClassID<WienerAntithetic>()
{
    static std::string name = "a";  // plain
    return name;
}

template<> std::string BaseClassName<WienerAntithetic>()
{
    return ClassName<WienerBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<WienerBase, WienerAntithetic> RegisterWienerAntithetic;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	onstructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

WienerAntithetic::WienerAntithetic()
:   N_(IO_Manager::Instance().GetValue<long>(ParameterN()))
,   anti_(false)   // so FillUpPath() can set it false
{    
	double Tmax = IO_Manager::Instance().GetValue<double>(ParameterTmax());
    dt_ =  Tmax / N_;  						  // time step
    rt_ = std::sqrt(dt_);            		  // speed up

    spare_path_.resize(0, N_);
    
    pln_ = new WienerPlain;
}

WienerAntithetic::~WienerAntithetic()
{
    delete pln_;
}
 						 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FillUpPath()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void WienerAntithetic::FillUpPath(VecB & path)
{
    long lb = path.LBound();
    long ub = path.UBound();
    
    if (lb != 0 || ub != N_) throw std::runtime_error("WienerAntithetic::FillUpPath:  bad bounds");

    if (anti_)
    {
		path = spare_path_;
		anti_ = !anti_;

		return;
	}

    pln_->FillUpPath(path);
    spare_path_ = -path;    
    
	anti_ = !anti_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FillUpSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void WienerAntithetic::FillUpSlice(const double T, VecB & slice) const
{
    long lb = slice.LBound();
    long ub = slice.UBound();
    
    double rT = std::sqrt(N_ * dt_);
    
    for(long i = lb;  i <= ub; ++i)     // uncorrelated random sample for time T
    {
	    double w = rv::GetNormalVariate();
       	slice[i] = w * rT;
    }
}
                 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	next_z()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double WienerAntithetic::next_z(double z)
{
    double w = rv::GetNormalVariate();
    return z + rt_*w;  // adds a Wiener increment
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string WienerAntithetic::GetName() const {return ClassName<WienerAntithetic>();}
std::string WienerAntithetic::GetID() const {return ClassID<WienerAntithetic>();}
std::string WienerAntithetic::GetBaseClassName() const {return BaseClassName<WienerAntithetic>();}
std::string WienerAntithetic::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  N =   " + lv::DoubleToString(N_) + '\n';
    nm += "  T =   " + lv::DoubleToString(N_*dt_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX







