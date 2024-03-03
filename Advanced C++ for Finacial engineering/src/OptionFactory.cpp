//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  OptionFactory.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>
#include <map>
#include <stdexcept>
#include <iostream>

#include "OptionFactory.h"

#include "ConfigurationManager.h"
#include "MonitorManager.h"
#include "CreateableBase.h"
#include "Parameters.h"
#include "OptionBase.h"

//class OptionBase;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionFactory::OptionFactory()
:   mon_(0)
{
	CreateObjectsMap_.clear();
	IDLookUpMap_.clear();
}

OptionFactory & OptionFactory::Instance()
{
    static OptionFactory this_factory;
	return this_factory;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Register with Factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXvXXXXXXXXXXXXXXXXXX

void OptionFactory::RegisterObject
(
	const std::string & Object_ID,
	const std::string & Object_name, 
	OptionFactory::DerivedClassCreator CreateThisObject
)
{
	CreatePair this_pair(Object_name, CreateThisObject);
	CreateObjectsMap_.insert(this_pair);
	
	LookUpPair that_pair(Object_ID, Object_name);
	IDLookUpMap_.insert(that_pair);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  CreateObject
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionBase * OptionFactory::Create(const std::string & otype, const Parameters & oparas)
{
	LookUpMap::const_iterator i = IDLookUpMap_.find(otype);
	if (i == IDLookUpMap_.end())
	{
		throw std::runtime_error("OptionFactory:  ID " + otype + " not found");
	}

	std::string nm = i->second;

	CreateMap::const_iterator j = CreateObjectsMap_.find(nm);
	if (j == CreateObjectsMap_.end())
	{
		throw std::runtime_error("OptionFactory:  Name " + nm + " not found");
	}

	OptionBase * CreatedObj = j->second(oparas);
	DoMonitorOutput(*CreatedObj);
	return CreatedObj;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXvXXXXXXXXXXXXXXXXXXX
//  DoMonitorOutput()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void OptionFactory::DoMonitorOutput(OptionBase & obj)
{
	if (mon_ == 0) mon_ = ConfigurationManager::Instance().GetMonitor();  // does not own
	mon_->OutputCreateable(obj);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
