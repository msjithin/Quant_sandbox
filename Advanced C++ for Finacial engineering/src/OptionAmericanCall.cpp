//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionAmericanCall.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <algorithm>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "VecB.h"
#include "lib_val.h"

#include "OptionAmericanCall.h"

#include "OptionIOManager.h"
#include "OptionFactoryRegistration.h"
#include "CreateableBase.h"

#include "Parameters.h"
#include "ParameterX.h"
#include "ParameterT.h"
#include "ParameterOptionID.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<OptionAmericanCall>()
{
    return "OptionAmericanCall";
}

template<> std::string ClassID<OptionAmericanCall>()
{
    return "C";
}

template<> std::string BaseClassName<OptionAmericanCall>()
{
    return ClassName<OptionBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	OptionFactoryRegistration<OptionAmericanCall> RegisterOptionAmericanCall;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionAmericanCall::OptionAmericanCall(const Parameters & paras)
:   X_(paras.GetValue<double>(ParameterX()))
,   T_(paras.GetValue<double>(ParameterT()))
,	OptionID_(paras.GetValue<double>(ParameterOptionID()))
,   early_(true)
{}

void OptionAmericanCall::initialize(double Tmax, long N)
{
	SetTindex(Tmax, N);
}       
                
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoffPath()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionAmericanCall::ComputePayoffPath(const std::vector<double> & path) const
{
    throw std::runtime_error("OptionAmericanCall::ComputePayoffPath:  cannot compute");
    return 0.0;
}
                     
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoffSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void OptionAmericanCall::ComputePayoffSlice(const double T, const VecB & S, VecB & P) const
{
    long lb = S.LBound();
    long ub = S.UBound();

    for(long i = lb;  i <= ub; ++i)
    {
        P[i] = ComputePayoff(S[i]);
    }
}
                    
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoff()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionAmericanCall::ComputePayoff(const double S) const
{
    return std::max(S - X_, 0.0);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	IsExerciseTime()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool OptionAmericanCall::IsExerciseTime(double t) const
{
	 return true;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetTindex
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void OptionAmericanCall::SetTindex(double Tmax, long N) 
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

std::string OptionAmericanCall::GetName() const {return ClassName<OptionAmericanCall>();}
std::string OptionAmericanCall::GetID() const {return ClassID<OptionAmericanCall>();}
std::string OptionAmericanCall::GetBaseClassName() const {return BaseClassName<OptionAmericanCall>();}
std::string OptionAmericanCall::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  X = " + lv::DoubleToString(X_) + '\n';
    nm += "  T = " + lv::DoubleToString(T_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

