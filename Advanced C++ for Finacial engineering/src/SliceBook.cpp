//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     SliceBook.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <iostream>
#include "VecB.h"
#include "utility.h"

#include "SliceBook.h"

#include "OptionBook.h"
#include "SliceBase.h"

#include "AppFactory.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

SliceBook::SliceBook()
{}

SliceBook::~SliceBook()
{
    for(long i = 0; i != Nopts_; ++i)
    {
		delete slc_[i];
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     interface
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void SliceBook::SetBranches(BranchesBase & brch)
{
    brch_ = &brch;
}
       
void SliceBook::SetValues(const OptionBook & obook, const ProcessBase & prc)
{
	Nopts_ = obook.GetNOptions();
	slc_.resize(Nopts_);
	
    for(long i = 1; i <= Nopts_; ++i)
    {
        slc_[i-1] = AppFactory<SliceBase>::Instance().CreateObject();
		slc_[i-1]->SetBranches(*brch_);         // must SetBranches before SetValues

        OptionBase * opt = obook.GetOption(i);  // obook uses logical indexes
        slc_[i-1]->SetValues(*opt, prc);
   	}
   	
   	bound_ = slc_[0]->GetBound();
}

void SliceBook::SetSvalues(VecB & Svals)
{
    for(long i = 1; i <= Nopts_; ++i)
    {
        slc_[i-1]->SetSvalues(Svals);
   	}
}

void SliceBook::DoOneStep(VecB & S_vals, long j,  double c_time)
{
    for(long i = 0; i != Nopts_; ++i)
    {
    	slc_[i]->DoOneStep(S_vals, j, c_time);
	}
}

void SliceBook::GetOptionIDs(VecB & vals)
{
    vals.resize(1, Nopts_);
    
    for(long i = 0; i != Nopts_; ++i)
    {
        vals[i+1] = slc_[i]->GetOptionID();
	}
}

void SliceBook::GetValues(VecB & vals)
{
    vals.resize(1, Nopts_);
    
    for(long i = 0; i != Nopts_; ++i)
    {
        vals[i+1] = slc_[i]->GetOValue();
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

