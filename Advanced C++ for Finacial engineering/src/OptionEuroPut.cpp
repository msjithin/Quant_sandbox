//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionEuroPut.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <algorithm>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "VecB.h"
#include "lib_val.h"

#include "OptionEuroPut.h"

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

template<> std::string ClassName<OptionEuroPut>()
{
    return "OptionEuroPut";
}

template<> std::string ClassID<OptionEuroPut>()
{
    return "p";
}

template<> std::string BaseClassName<OptionEuroPut>()
{
    return ClassName<OptionBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	OptionFactoryRegistration<OptionEuroPut> RegisterOptionEuroPut;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionEuroPut::OptionEuroPut(const Parameters & paras)
:   X_(paras.GetValue<double>(ParameterX()))
,   T_(paras.GetValue<double>(ParameterT()))
,	OptionID_(paras.GetValue<double>(ParameterOptionID()))
,   early_(false)
{}

void OptionEuroPut::initialize(double Tmax, long N)
{
	SetTindex(Tmax, N);
}       
                
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoffPath()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionEuroPut::ComputePayoffPath(const std::vector<double> & path) const
{
    return ComputePayoff(path.back());
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputePayoffSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void OptionEuroPut::ComputePayoffSlice(const double T, const VecB & S, VecB & P) const
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

double OptionEuroPut::ComputePayoff(const double S) const
{
    return std::max(X_ - S, 0.0);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	IsExerciseTime()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

bool OptionEuroPut::IsExerciseTime(double t) const
{
	 return t == T_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetTindex
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void OptionEuroPut::SetTindex(double Tmax, long N) 
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

std::string OptionEuroPut::GetName() const {return ClassName<OptionEuroPut>();}
std::string OptionEuroPut::GetID() const {return ClassID<OptionEuroPut>();}
std::string OptionEuroPut::GetBaseClassName() const {return BaseClassName<OptionEuroPut>();}
std::string OptionEuroPut::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  X = " + lv::DoubleToString(X_) + '\n';
    nm += "  T = " + lv::DoubleToString(T_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

