//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	MC_method.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <vector>
#include <string>
#include "lib_val.h"
#include "VecB.h"

#include "MC_method.h"

#include "AppFactoryRegistration.h"
#include "AppFactory.h"
#include "ConfigurationManager.h"
#include "OutputManager.h"
#include "OptionOutputManager.h"
#include "MonitorManager.h"

#include "AccumulatorBook.h"
#include "OptionBook.h"
#include "ProcessBase.h"

#include "ParameterM.h"
#include "ParameterN.h"
#include "ParameterTmax.h"

#include "CreateableBase.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<MC_method>()
{
    return "MC_method";
}

template<> std::string ClassID<MC_method>()
{
    return "p";
}

template<> std::string BaseClassName<MC_method>()
{
    return ClassName<ValuationMethodBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<ValuationMethodBase, MC_method> RegisterMC_method;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

MC_method::MC_method()
:	acc_(0)
,   M_(IO_Manager::Instance().GetValue<long>(ParameterM()))
,   N_(IO_Manager::Instance().GetValue<long>(ParameterN()))
,   Tmax_(IO_Manager::Instance().GetValue<double>(ParameterTmax()))
,   interval_(50000)
{
	path_.resize(N_ + 1);
	
    mon_ = ConfigurationManager::Instance().GetMonitor();

   	acc_ = new AccumulatorBook;   								    // Owns
}

MC_method::~MC_method()
{
    delete acc_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetOutput()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MC_method::SetOutput(OutputManager & out) const
{}

void MC_method::SetOutput(OptionOutputManager & out) const
{
    VecB vals;
    VecB ses;
    VecB IDs;

	acc_->GetValues(vals);
    acc_->GetSEs(ses);
    acc_->GetOptionIDs(IDs);

    long lb = vals.LBound();
    long ub = vals.UBound();

    typedef std::pair<std::string, double> ValuePair;
    typedef std::vector<ValuePair> Results;
    
    Results res_;

    for(long i = lb; i <= ub; ++i)            // for each sample path
    {
		ValuePair this_pair1("Option ID      ", IDs[i]);
		ValuePair this_pair2("Option value   ", vals[i]);
		ValuePair this_pair3("Standard error ", ses[i]);
		
		res_.push_back(this_pair1);
		res_.push_back(this_pair2);
		res_.push_back(this_pair3);

        out.RegisterOutput(res_);
        
        res_.clear();
    }
}
           
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	run()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MC_method::run(OptionBook & optb, ProcessBase & prc)
{
	acc_->reset(optb);
	
	optb.initialize(Tmax_, N_);
	
	long N_opts = optb.GetNOptions();
    
	VecB results(1, N_opts);

    for(long j = 1; j <= M_; ++j)
    {
        mon_->OutputCounter(j, M_);
        
        prc.FillUpPath(path_);
                
        optb.ReceivePath(path_, results);

        acc_->update(results);               // accumulate payoffs
    }
}
 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string MC_method::GetName() const {return ClassName<MC_method>();}
std::string MC_method::GetID() const {return ClassID<MC_method>();}
std::string MC_method::GetBaseClassName() const {return BaseClassName<MC_method>();}
std::string MC_method::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  M = " + lv::DoubleToString(M_) + '\n';
    nm += "  N = " + lv::DoubleToString(N_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
