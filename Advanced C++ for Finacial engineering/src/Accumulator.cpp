//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Accumulator.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <cmath>
#include <stdexcept>
#include "lib_val.h"

#include "Accumulator.h"

#include "IO_Manager.h"
#include "AppFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterR.h"
#include "ParameterT.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<Accumulator>()
{
    return "Accumulator";
}

template<> std::string ClassID<Accumulator>()
{
    return "p";
}

template<> std::string BaseClassName<Accumulator>()
{
    return ClassName<AccumulatorBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<AccumulatorBase, Accumulator> RegisterAccumulator;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Accumulator::Accumulator()
:   acc_vals_(0.0)
,   acc_squs_(0.0)
,   M_(0)
{}

void Accumulator::SetT(const double & T)
{
	double r = IO_Manager::Instance().GetValue<double>(ParameterR());
    discount_ = std::exp(-r*T);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	reset()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Accumulator::reset()
{
	acc_vals_ = 0.0;
	acc_squs_ = 0.0;
	M_ = 0;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Update()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Accumulator::Update(double payoff)
{
    acc_vals_ += payoff;
    acc_squs_ += payoff*payoff;
    ++M_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	getters
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Accumulator::GetValue() const
{
    return discount_*acc_vals_/M_;
}

double Accumulator::GetSE() const
{    
    double radix = acc_squs_ - acc_vals_*acc_vals_/M_;
    
    if(radix < 0) throw std::runtime_error("Accumulator::GetSE():  Radix is negative");
    
    return discount_*std::sqrt(radix)/M_;
}
                
long Accumulator::GetM() const
{
    return M_;
}
                
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string Accumulator::GetName() const {return ClassName<Accumulator>();}
std::string Accumulator::GetID() const {return ClassID<Accumulator>();}
std::string Accumulator::GetBaseClassName() const {return BaseClassName<Accumulator>();}
std::string Accumulator::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  M_ =                   " + lv::DoubleToString(M_) + '\n';
    nm += "  accumulated values =   " + lv::DoubleToString(acc_vals_) + '\n';
    nm += "  accumulated squares =  " + lv::DoubleToString(acc_squs_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
