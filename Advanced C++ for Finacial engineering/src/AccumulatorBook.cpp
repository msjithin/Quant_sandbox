//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     AccumulatorBook.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <iostream>
#include "VecB.h"

#include "AccumulatorBook.h"

#include "OptionBook.h"
#include "AccumulatorBase.h"

#include "AppFactory.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

AccumulatorBook::AccumulatorBook()
{}

AccumulatorBook::~AccumulatorBook()
{
    for(long i = 0; i != Nopts_; ++i)
    {
		delete acc_[i];
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     interface
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void AccumulatorBook::reset(OptionBook & obook)
{
	Nopts_ = obook.GetNOptions();
	acc_.resize(Nopts_);
	
    for(long i = 0; i != Nopts_; ++i)
    {
        acc_[i] = AppFactory<AccumulatorBase>::Instance().CreateObject();
        acc_[i]->reset();
        
        double T = obook.GetT(i+1);
        long ID = obook.GetOptionID(i+1);
        
        acc_[i]->SetT(T);
        acc_[i]->SetOptionID(ID);
	}
}

void AccumulatorBook::update(const VecB & results)
{
    for(long i = 0; i != Nopts_; ++i)
    {
        acc_[i]->Update(results[i+1]);
	}
}

void AccumulatorBook::GetValues(VecB & vals)
{
    vals.resize(1, Nopts_);
    
    for(long i = 0; i != Nopts_; ++i)
    {
        vals[i+1] = acc_[i]->GetValue();
	}
}

void AccumulatorBook::GetSEs(VecB & ses)
{
    ses.resize(1, Nopts_);

    for(long i = 0; i != Nopts_; ++i)
    {
        ses[i+1] = acc_[i]->GetSE();
	}
}

void AccumulatorBook::GetOptionIDs(VecB & ses)
{
    ses.resize(1, Nopts_);

    for(long i = 0; i != Nopts_; ++i)
    {
        ses[i+1] = acc_[i]->GetOptionID();
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

