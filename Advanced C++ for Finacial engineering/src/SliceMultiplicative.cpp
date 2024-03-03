//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SliceMultiplicative.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include "lib_val.h"

#include "VecB.h"
#include "utility.h"

#include "SliceMultiplicative.h"

#include "IO_Manager.h"
#include "AppFactoryRegistration.h"
#include "CreateableBase.h"

#include "BranchesBase.h"
#include "ProcessBase.h"
#include "OptionBase.h"

#include "ParameterN.h"
#include "ParameterTmax.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<SliceMultiplicative>()
{
    return "SliceMultiplicative";
}

template<> std::string ClassID<SliceMultiplicative>()
{
    return "m";
}

template<> std::string BaseClassName<SliceMultiplicative>()
{
    return ClassName<SliceBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<SliceBase, SliceMultiplicative> RegisterSliceMultiplicative;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

SliceMultiplicative::SliceMultiplicative()
:   N_(IO_Manager::Instance().GetValue<long>(ParameterN()))
,   Tmax_(IO_Manager::Instance().GetValue<long>(ParameterTmax()))
,   this_value_(0)
,   next_value_(0)
{
    this_value_ = new VecB;
    next_value_ = new VecB;

    dt_ = Tmax_/N_;
}
                     
SliceMultiplicative::~SliceMultiplicative()
{
	delete this_value_;
	delete next_value_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetBranches()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicative::SetBranches(const BranchesBase & ptb)
{
    prb_ = &ptb;
    
    upbranches_ = prb_->GetUpBranches();
    ub_max_ = N_ * upbranches_;
        
    this_value_->resize(-ub_max_, ub_max_);
    next_value_->resize(-ub_max_, ub_max_);
    payoffs_.resize(-ub_max_, ub_max_);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetValues()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicative::SetValues(const OptionBase & opt, const ProcessBase & proc)
{
    opt_ = &opt;
    prc_ = &proc;
		
    dis_dt_ = prc_->GetDtDiscount(dt_);
    
    T_ = opt_->GetT();
    ID_ = opt_->GetOptionID();
    
    SetNT();  // check if T_ is a whole number of time steps,  set NT_
}
 
void SliceMultiplicative::SetSvalues(VecB & S_vals)
{
    S_values_ = &S_vals;
    opt_->ComputePayoffSlice(current_t_, *S_values_, *this_value_);  
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  DoOneStep()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicative::DoOneStep(VecB & S_vals, long j,  double c_time)
{
	if (NT_ < j) return;
	if (NT_ == j)
	{
        opt_->ComputePayoffSlice(current_t_, *S_values_, *this_value_); 
		return;
	}
	
    roll_back(j);
    Early_update(S_vals, j, c_time);
    roll_over(j);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  roll_back()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicative::roll_back(long j)
{
	long ub = j*upbranches_;
    for(long i = -ub; i <= ub; ++i)
    {
        (*next_value_)[i] = dis_dt_ * prb_->NextValue(i, *this_value_);
    }            
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXvXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Early_update()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicative::Early_update(VecB & S_vals, long j, double current_t_)
{
    if (!Early_exercise_) return;

    S_values_ = &S_vals;
	if (opt_->IsExerciseTime(current_t_)) Bermudan_update(j);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Bermudan_update()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicative::Bermudan_update(long j)
{
	long ub = j*upbranches_;
    for(long i = -ub; i <= ub; ++i)
    {
        payoffs_[i] = opt_->ComputePayoff((*S_values_)[i]);

        if (payoffs_[i] > (*next_value_)[i])
		{
  		    (*next_value_)[i] = payoffs_[i];
		}
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  roll_over().  swap over the two pointers:  rolls over to the next time.
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicative::roll_over(long j){std::swap(this_value_, next_value_);}
 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetNT
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicative::SetNT()
{
	if(!lv::IsInteger(T_/dt_, dt_/100.0))
	{
        std::ostringstream prompt;
        prompt << "Option " << ID_ << ".  T does not lie on a time step";
		throw std::runtime_error(prompt.str());
	}
	
	NT_ = long(T_/dt_ + dt_/100.0);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string SliceMultiplicative::GetName() const {return ClassName<SliceMultiplicative>();}
std::string SliceMultiplicative::GetID() const {return ClassID<SliceMultiplicative>();}
std::string SliceMultiplicative::GetBaseClassName() const {return BaseClassName<SliceMultiplicative>();}
std::string SliceMultiplicative::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  N =  " + lv::DoubleToString(N_) + '\n';
    nm += "  current_t =  " + lv::DoubleToString(current_t_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX







