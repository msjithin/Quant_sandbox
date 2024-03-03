//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Branches7.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <cmath>
#include "VecB.h"
#include "lib_val.h"

#include "Branches7.h"

#include "IO_Manager.h"
#include "AppFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterKappa.h"
#include "ParameterTmax.h"
#include "ParameterN.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<Branches7>()
{
    return "Branches7";
}

template<> std::string ClassID<Branches7>()
{
    return "7";
}

template<> std::string BaseClassName<Branches7>()
{
    return ClassName<BranchesBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<BranchesBase, Branches7> RegisterBranches7;

	const long NumBranches = 7;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Branches7::Branches7()
:   CRITICAL_KAPPA_(1.43276057073026)
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

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ComputeProbs()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Branches7::ComputeProbs()
{
    double k1 = kappa_;
    double k2 = kappa_ * kappa_;
    double k3 = kappa_ * kappa_ * kappa_;

    probs_.resize(-up_branches_, up_branches_);
    probs_[3] = (15.0 - 15.0 * k1 + 4.0 * k2) / (720.0 * k3);
    probs_[2] = (-15.0 + 30.0 * k1 - 9.0 * k2) / (120.0 * k3);
    probs_[1] = (15.0 - 39.0 * k1 + 36.0 * k2) / (48.0 * k3);
    probs_[0] = 1.0 - 2.0 * (probs_[1] + probs_[2] + probs_[3]);
    probs_[-1] = probs_[1];
    probs_[-2] = probs_[2];
    probs_[-3] = probs_[3];
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  const double Branches3::operator[](const long i) const
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Branches7::vsqubra(const long i) const
{
    return probs_[i];  //called with logical indexes
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  NextValue()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double Branches7::NextValue(long j, VecB & vec) const
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

std::string Branches7::GetName() const {return ClassName<Branches7>();}
std::string Branches7::GetID() const {return ClassID<Branches7>();}
std::string Branches7::GetBaseClassName() const {return BaseClassName<Branches7>();}
std::string Branches7::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  kappa =   " + lv::DoubleToString(kappa_) + '\n';
    nm += "  dz = " + lv::DoubleToString(dz_) + '\n';
    nm += "  mid-prob =  " + lv::DoubleToString(probs_[0]) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

