//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SliceMultiplicativePrune.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <cmath>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <sstream>

#include "utility.h"
#include "lib_val.h"
#include "VecB.h"

#include "SliceMultiplicativePrune.h"

#include "IO_Manager.h"

#include "BranchesBase.h"
#include "ProcessBase.h"
#include "OptionBase.h"

#include "ParameterN.h"
#include "ParameterTmax.h"
#include "ParameterPrune.h"

#include "CreateableBase.h"
#include "AppFactoryRegistration.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  traits
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<SliceMultiplicativePrune>()
{
    static std::string name = "SliceMultiplicativePrune";
    return name;
}

template<> std::string ClassID<SliceMultiplicativePrune>()
{
    static std::string name = "p";
    return name;
}

template<> std::string BaseClassName<SliceMultiplicativePrune>()
{
    return ClassName<SliceBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<SliceBase, SliceMultiplicativePrune> RegisterSliceMultiplicativePrune;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

SliceMultiplicativePrune::SliceMultiplicativePrune()
:   N_(IO_Manager::Instance().GetValue<double>(ParameterN()))
,   Tmax_(IO_Manager::Instance().GetValue<long>(ParameterTmax()))
,   lambda_(IO_Manager::Instance().GetValue<double>(ParameterPrune()))
,   N_prune_(N_)
,   this_value_(0)
,   next_value_(0)
{
    this_value_ = new VecB;
    next_value_ = new VecB;

    dt_ = Tmax_/N_;
}
                     
SliceMultiplicativePrune::~SliceMultiplicativePrune()
{
	delete this_value_;
	delete next_value_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetBranches()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicativePrune::SetBranches(const BranchesBase & ptb)
{
    prb_ = &ptb;
    
    upbranches_ = prb_->GetUpBranches();        
    double dz = prb_->Getdz();
    
    ub_cur_ = N_ * upbranches_;  // default number of up-steps

    N_prune_ = std::min(ub_cur_, long(lambda_*std::sqrt(Tmax_)/dz));  // to cap at ub_cur_
    j_prune_ = long(std::floor((N_prune_/upbranches_)));  		   // floor on j
    N_prune_ = j_prune_ * upbranches_;   						   // make multiple of upbranches_
    
    ub_cur_ = N_prune_ + upbranches_;  //so that never run out of bounds.

    this_value_->resize(-ub_cur_, ub_cur_);
    next_value_->resize(-ub_cur_, ub_cur_);
    payoffs_.resize(-ub_cur_, ub_cur_);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetValues()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicativePrune::SetValues(const OptionBase & opt, const ProcessBase & proc)
{
    opt_ = &opt;
    prc_ = &proc;

    dis_dt_ = prc_->GetDtDiscount(dt_);
        
    T_ = opt_->GetT();
    ID_ = opt_->GetOptionID();
    
    SetNT();  // check if T_ is a whole number of time steps.  sets NT_
}
 
void SliceMultiplicativePrune::SetSvalues(VecB & S_vals)
{
    S_values_ = &S_vals;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  DoOneStep()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicativePrune::DoOneStep(VecB & S_vals, long j,  double c_time)
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

void SliceMultiplicativePrune::roll_back(long j)
{
    j = std::min(j, j_prune_);

	long ub = j*upbranches_;
    for(long i = -ub; i <= ub; ++i)
    {
        (*next_value_)[i] = dis_dt_ * prb_->NextValue(i, *this_value_);
    }            
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Bermudan_update()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicativePrune::Early_update(VecB & S_vals, long j, double current_t_)
{
    if (!Early_exercise_) return;

    S_values_ = &S_vals;
	if (opt_->IsExerciseTime(current_t_)) Bermudan_update(j);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Bermudan_update()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicativePrune::Bermudan_update(long j)
{
    j = std::min(j, j_prune_);

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

void SliceMultiplicativePrune::roll_over(long j){std::swap(this_value_, next_value_);}
 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetNT
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceMultiplicativePrune::SetNT()
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
//	CreateableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string SliceMultiplicativePrune::GetName() const {return ClassName<SliceMultiplicativePrune>();}
std::string SliceMultiplicativePrune::GetID() const {return ClassID<SliceMultiplicativePrune>();}
std::string SliceMultiplicativePrune::GetBaseClassName() const {return BaseClassName<SliceMultiplicativePrune>();}
std::string SliceMultiplicativePrune::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  N =  " + lv::DoubleToString(N_) + '\n';
    nm += "  current_t =  " + lv::DoubleToString(current_t_) + '\n';
    nm += "  lambda =     " + lv::DoubleToString(lambda_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX







