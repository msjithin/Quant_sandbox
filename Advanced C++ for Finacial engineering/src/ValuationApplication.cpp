//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	ValuationApplication.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "lib_val.h"

#include "ValuationApplication.h"

#include "ConfigurationManager.h"
#include "MonitorManager.h"
#include "OutputManager.h"
#include "OptionOutputManager.h"
#include "OptionIOManager.h"

#include "OptionBook.h"
#include "ProcessBase.h"
#include "ValuationMethodBase.h"

#include "AppFactory.h"
#include "AppFactoryRegistration.h"

#include "CreateableBase.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<ValuationApplication>()
{
    return "ValuationApplication";
}

template<> std::string ClassID<ValuationApplication>()
{
    return "v";
}

template<> std::string BaseClassName<ValuationApplication>()
{
    return ClassName<ApplicationBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<ApplicationBase, ValuationApplication> RegisterValuationApplication;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

ValuationApplication::ValuationApplication()
:	opt_(0)
,	prc_(0)
,	mth_(0)
{
    opt_ = new OptionBook;
    prc_ = AppFactory<ProcessBase>::Instance().CreateObject();
    mth_ = AppFactory<ValuationMethodBase>::Instance().CreateObject();
    
    out_ = OptionIOManager::Instance().GetOptionOutputManager();  //does not own this

    mon_ = ConfigurationManager::Instance().GetMonitor();
}

ValuationApplication::~ValuationApplication()
{
    delete opt_;
    delete prc_;
    delete mth_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetOutput()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ValuationApplication::SetOutput(OutputManager & out) const
{
    out_->Clear();
    mth_->SetOutput(*out_);  // No output to out.
    out_->DoOutput();
}
           
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	run
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ValuationApplication::run()
{
	mon_->OutputBanner("Option valuation application");
	
    mth_->run(*opt_, *prc_);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string ValuationApplication::GetName() const {return ClassName<ValuationApplication>();}
std::string ValuationApplication::GetID() const {return ClassID<ValuationApplication>();}
std::string ValuationApplication::GetBaseClassName() const {return BaseClassName<ValuationApplication>();}
std::string ValuationApplication::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  " +  ((prc_ == 0) ? "Process not set" : "Process = " + prc_->GetName()) + '\n';
    nm += "  " +  ((mth_ == 0) ? "Method not set " : "Method =  " + mth_->GetName()) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


















