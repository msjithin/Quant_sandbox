//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Application_Wrapper.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "Application_Wrapper.h"

#include "AppFactory.h"

#include "ConfigurationManager.h"
#include "MonitorManager.h"

#include "IO_Manager.h"
#include "OutputManager.h"

#include "StopWatch.h"
#include "ApplicationBase.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Application_Wrapper::Application_Wrapper()
:	app_(0)
{
	mon_ = ConfigurationManager::Instance().GetMonitor();
	out_ = IO_Manager::Instance().GetOutputManager();
	
    stw_ = new StopWatch;                  			// owns this pointer
    app_ = AppFactory<ApplicationBase>::Instance().CreateObject();	// owns this pointer
}
                                          
Application_Wrapper::~Application_Wrapper()
{
   delete stw_;
   delete app_;
}
                                           
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	run
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Application_Wrapper::run()
{
	mon_->OutputBanner("Assessment:  Valuing a book on a lattice,  NJW");
    
    stw_->StartStopWatch();
    
    out_->Clear();

    	app_->run();
    
    	app_->SetOutput(*out_);
        
    	out_->RegisterOutput("Time taken", stw_->GetTime());

    out_->DoOutput();
    
    mon_->OutputBanner("Finished");
}
 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
