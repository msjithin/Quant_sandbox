//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	LatticeMethod.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <cmath>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "utility.h"
#include "lib_val.h"
#include "VecB.h"

#include "LatticeMethod.h"

#include "ConfigurationManager.h"
#include "AppFactory.h"
#include "AppFactoryRegistration.h"
#include "CreateableBase.h"

#include "OutputManager.h"
#include "OptionOutputManager.h"
#include "MonitorManager.h"

#include "BranchesBase.h"
#include "SliceBook.h"

#include "ProcessBase.h"
#include "OptionBook.h"
#include "OptionBase.h"

#include "ParameterN.h"
#include "ParameterTmax.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<LatticeMethod>()
{
    return "LatticeMethod";
}

template<> std::string ClassID<LatticeMethod>()
{
    return "l";
}

template<> std::string BaseClassName<LatticeMethod>()
{
    return ClassName<ValuationMethodBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<ValuationMethodBase, LatticeMethod> RegisterLatticeMethod;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

LatticeMethod::LatticeMethod()
:	prb_(0)
,   slc_(0)
,   S_values_(0)
,   N_(IO_Manager::Instance().GetValue<long>(ParameterN()))
,   Tmax_(IO_Manager::Instance().GetValue<double>(ParameterTmax()))
,   interval_(10)
{
    mon_ = ConfigurationManager::Instance().GetMonitor();	    // does not own
    prb_ = AppFactory<BranchesBase>::Instance().CreateObject(); // Owns
    
    slc_ = new SliceBook;
    S_values_ = new VecB;

    slc_->SetBranches(*prb_);
    
    dt_ = Tmax_/N_;
    current_t_ = Tmax_;
}
                     
LatticeMethod::~LatticeMethod()
{
    delete prb_;
    delete slc_;
    delete S_values_;
}
                     
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	run()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void LatticeMethod::run(OptionBook & optb, ProcessBase & proc)
{
    scale_s_ = proc.GetSScale(dt_);
    
	optb.initialize(Tmax_, N_);

    slc_->SetValues(optb, proc);

    ComputeSvalues(proc);
    slc_->SetSvalues(*S_values_); 
    
    for(long j = N_ - 1; j >= 0; --j)
    {
        mon_->OutputCounter(j, 0, interval_);
        
        current_t_ -= dt_;
        
    	roll_back_S(j);
        
    	slc_->DoOneStep(*S_values_, j, current_t_);    	
    }
}
 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  ComputeSvalues()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void LatticeMethod::ComputeSvalues(ProcessBase & proc)
{
    double dz = prb_->Getdz();
    long adj = slc_->GetBound();

    VecB Z_values_(-adj, adj);  // Wiener values for the final time
    for(long i = -adj; i <= adj; ++i)
    {
        Z_values_[i] = i*dz;
    }

    S_values_->resize(-adj, adj);
    proc.FillUpSlice(current_t_, *S_values_, Z_values_);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  roll_back_S()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void LatticeMethod::roll_back_S(long j)
{
	long ub = slc_->GetBound();
    for(long i = -ub; i <= ub; ++i) (*S_values_)[i] *= scale_s_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetOutput()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void LatticeMethod::SetOutput(OutputManager & out) const
{}

void LatticeMethod::SetOutput(OptionOutputManager & out) const
{
    VecB vals;
    VecB IDs;

	slc_->GetValues(vals);
    slc_->GetOptionIDs(IDs);

    long lb = vals.LBound();
    long ub = vals.UBound();

    typedef std::pair<std::string, double> ValuePair;
    typedef std::vector<ValuePair> Results;
    
    Results res_;

    for(long i = lb; i <= ub; ++i)            // for each sample path
    {
		ValuePair this_pair1("Option ID      ", IDs[i]);
		ValuePair this_pair2("Option value   ", vals[i]);
		
		res_.push_back(this_pair1);
		res_.push_back(this_pair2);

        out.RegisterOutput(res_);
        
        res_.clear();
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string LatticeMethod::GetName() const {return ClassName<LatticeMethod>();}
std::string LatticeMethod::GetID() const {return ClassID<LatticeMethod>();}
std::string LatticeMethod::GetBaseClassName() const {return BaseClassName<LatticeMethod>();}
std::string LatticeMethod::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  " + ((prb_ == 0) ? "Branches not set" : "Branches = " + prb_->GetName()) + '\n';
    nm += "  Tmax = " + lv::DoubleToString(Tmax_) + '\n';
    nm += "  N = " + lv::DoubleToString(N_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX







