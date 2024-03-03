//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionAverageRate.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <algorithm>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "VecB.h"
#include "lib_val.h"

#include "OptionAverageRate.h"

#include "OptionIOManager.h"
#include "OptionFactoryRegistration.h"
#include "CreateableBase.h"

#include "Parameters.h"
#include "ParameterX.h"
#include "ParameterT.h"
#include "ParameterOptionID.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<OptionAverageRate>()
{
    return "OptionAverageRate";
}

template<> std::string ClassID<OptionAverageRate>()
{
    return "a";
}

template<> std::string BaseClassName<OptionAverageRate>()
{
    return ClassName<OptionBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	OptionFactoryRegistration<OptionAverageRate> RegisterOptionAverageRate;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionAverageRate::OptionAverageRate(const Parameters & paras)
:   X_(paras.GetValue<double>(ParameterX()))
,   T_(paras.GetValue<double>(ParameterT()))
,	OptionID_(paras.GetValue<double>(ParameterOptionID()))
,   early_(false)
{}

void OptionAverageRate::initialize(double Tmax, long N)
{
	SetTindex(Tmax, N);
}       
                
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoffPath()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionAverageRate::ComputePayoffPath(const std::vector<double> & path) const
{
    double av = 0.0;
    long N = path.size();

    for(long i = 0; i < N; ++i)
    {
        av += path[i];
    }
    av = av/N;

    return ComputePO(av);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoff()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionAverageRate::ComputePayoff(const double S) const
{
    throw std::runtime_error("OptionAverageRate::ComputePayoff:  cannot compute");
    return 0.0;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoffSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void OptionAverageRate::ComputePayoffSlice(const double T, const VecB & S, VecB & P) const
{
    throw std::runtime_error("OptionAverageRate::ComputePayoffSlice:  cannot compute");
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePO()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionAverageRate::ComputePO(const double S) const
{
    return std::max(S - X_, 0.0);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	IsExerciseTime()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool OptionAverageRate::IsExerciseTime(double t) const
{
	 return t == T_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetTindex
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void OptionAverageRate::SetTindex(double Tmax, long N) 
{
    if (T_ > Tmax) throw std::runtime_error(GetName() + "::SetTindex.  T greater than Tmax");

	double dt = Tmax/N;
	if(!lv::IsInteger(T_/dt, dt/100.0))
	{
        std::ostringstream prompt;
        prompt << GetName() << ",  " << OptionID_ << ".  T does not lie on a time step";
		throw std::runtime_error(prompt.str());
	}
	
	Tindex_ = long(T_/dt + dt/100.0);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string OptionAverageRate::GetName() const {return ClassName<OptionAverageRate>();}
std::string OptionAverageRate::GetID() const {return ClassID<OptionAverageRate>();}
std::string OptionAverageRate::GetBaseClassName() const {return BaseClassName<OptionAverageRate>();}
std::string OptionAverageRate::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  X = " + lv::DoubleToString(X_) + '\n';
    nm += "  T = " + lv::DoubleToString(T_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
