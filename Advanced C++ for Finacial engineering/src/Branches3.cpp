//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Branches3.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <cmath>
#include <vector>
#include "VecB.h"
#include "lib_val.h"

#include "Branches3.h"

#include "IO_Manager.h"
#include "AppFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterKappa.h"
#include "ParameterTmax.h"
#include "ParameterN.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<Branches3>()
{
    return "Branches3";
}

template<> std::string ClassID<Branches3>()
{
    return "3";
}

template<> std::string BaseClassName<Branches3>()
{
    return ClassName<BranchesBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<BranchesBase, Branches3> RegisterBranches3;
	
	const long NumBranches = 3;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Branches3::Branches3()
:   CRITICAL_KAPPA_(3.0)
,   branches_(NumBranches)
,   up_branches_(long(0.5*(branches_ - 1)))
,   kappa_(IO_Manager::Instance().GetValue<double>(ParameterKappa()))
{
	if (kappa_ == 0) kappa_ = CRITICAL_KAPPA_;
	
    long N = IO_Manager::Instance().GetValue<long>(ParameterN());
    double Tmax = IO_Manager::Instance().GetValue<double>(ParameterTmax());
	double dt = Tmax/N;

    dz_ = std::sqrt(kappa_*dt);
    ComputeProbs();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	void Branches3::ComputeProbs()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Branches3::ComputeProbs()
{
    probs_.resize(-up_branches_, up_branches_);
    probs_[1] = 0.5/kappa_;                 //branches hardwired at three
	probs_[0] = (kappa_ - 1)/kappa_;        //index of middle branch
    probs_[-1] = probs_[1];
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  const double Branches3::operator[](const long i) const
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Branches3::vsqubra(const long i) const
{
    return probs_[i];        //called with logical indexes
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  NextValue()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Branches3::NextValue(long j, VecB & vec) const
{
    double v = 0.0;
    
    for(long k = -up_branches_; k <= up_branches_; ++k)
    {
        v += probs_[k] * vec[j + k];
    }
    
    return v;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string Branches3::GetName() const {return ClassName<Branches3>();}
std::string Branches3::GetID() const {return ClassID<Branches3>();}
std::string Branches3::GetBaseClassName() const {return BaseClassName<Branches3>();}
std::string Branches3::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  kappa =   " + lv::DoubleToString(kappa_) + '\n';
    nm += "  dz = " + lv::DoubleToString(dz_) + '\n';
    nm += "  mid-prob =  " + lv::DoubleToString(probs_[0]) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

