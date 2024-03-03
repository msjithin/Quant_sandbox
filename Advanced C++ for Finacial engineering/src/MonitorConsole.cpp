//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	MonitorConsole.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>
#include "utility.h"

#include "MonitorConsole.h"

#include "ParameterInterval.h"

#include "ConfigurationFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterMonitorPrintObj.h"
#include "ParameterMonitorPrintParas.h"
#include "ParameterMonitorPrintIO.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<MonitorConsole>()
{
    static std::string name = "MonitorConsole";
    return name;
}

template<> std::string ClassID<MonitorConsole>()
{
    static std::string name = "c";
    return name;
}

template<> std::string BaseClassName<MonitorConsole>()
{
    return ClassName<MonitorBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	ConfigurationFactoryRegistration<MonitorBase, MonitorConsole> RegisterMonitorConsole;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

MonitorConsole::MonitorConsole()
:   interval_(SetUpManager::Instance().GetValue<long>(ParameterInterval()))
,	Obj_desc_(SetUpManager::Instance().GetValue<bool>(ParameterMonitorPrintObj()))
,	Obj_paras_(SetUpManager::Instance().GetValue<bool>(ParameterMonitorPrintParas()))
,	IO_desc_(SetUpManager::Instance().GetValue<bool>(ParameterMonitorPrintIO()))
{}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Output strings
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonitorConsole::OutputBanner(const std::string & strg)
{
	ut::OutputLine(strg);
}

void MonitorConsole::OutputLine(const std::string & strg)
{
	ut::OutputLine(strg);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OutputCounter()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonitorConsole::OutputCounter(long i, long N)
{
	ut::OutputCounter(i, N, interval_);
}

void MonitorConsole::OutputCounter(long i, long N, long interval)
{
	ut::OutputCounter(i, N, interval);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OutputCreateable()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonitorConsole::OutputCreateable(CreateableBase & obj) const
{
	if (Obj_desc_ && !Obj_paras_) ut::OutputLine(obj.GetBaseClassName() + " is " + obj.GetName());
	if (Obj_paras_) ut::OutputLine(obj.GetStringify());
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OutputIOspecs()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void MonitorConsole::OutputIOspecs(const std::string & strg) const
{
	if (IO_desc_) ut::OutputLine(strg);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

