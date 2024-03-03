//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  AppFactory.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>
#include <map>
#include <stdexcept>

#include "IO_Manager.h"
#include "CreateableBase.h"
#include "IdentifierType.h"
#include "ConfigurationManager.h"
#include "MonitorManager.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> AppFactory<Base_class>::AppFactory()
:   mon_(0)
{
	CreateObjectsMap_.clear();
	IDLookUpMap_.clear();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Register with Factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> void AppFactory<Base_class>::RegisterObject
(
	const std::string & Object_ID,
	const std::string & Object_name, 
	AppFactory<Base_class>::DerivedClassCreator CreateThisObject
)
{
	CreateObjectsMap_[Object_name] = CreateThisObject;
	IDLookUpMap_[Object_ID] = Object_name;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  CreateObject
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> Base_class * AppFactory<Base_class>::CreateObject()
{
	IO_Manager & io = IO_Manager::Instance();
	std::string o_type = io.GetValue<std::string>(IdentifierType(ClassName<Base_class>()));

	typename LookUpMap::const_iterator i = IDLookUpMap_.find(o_type);
	if (i == IDLookUpMap_.end())
	{
		throw std::runtime_error("AppFactory:  Base " + ClassName<Base_class>() + ", ID " + o_type + " not found");
	}

	std::string nm = i->second;

	typename CreateMap::const_iterator j = CreateObjectsMap_.find(nm);
	if (j == CreateObjectsMap_.end())
	{
		throw std::runtime_error("AppFactory:  Base " + ClassName<Base_class>() + ", Name " + nm + " not found");
	}

	Base_class * CreatedObj = j->second();
	DoMonitorOutput(*CreatedObj);
	return CreatedObj;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  DoMonitorOutput()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> void AppFactory<Base_class>::DoMonitorOutput(Base_class & obj)
{
	if (mon_ == 0) mon_ = ConfigurationManager::Instance().GetMonitor();  // does not own
	mon_->OutputCreateable(obj);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
