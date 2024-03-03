//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  IOFactory.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>
#include <map>
#include <stdexcept>

#include "MonitorManager.h"

#include "IOFactory.h"
#include "IdentifierType.h"
#include "CreateableBase.h"

#include "ConfigurationManager.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> IOFactory<Base_class>::IOFactory()
:   mon_(0)
{
	CreateObjectsMap_.clear();
	IDLookUpMap_.clear();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Register with IOFactory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> void IOFactory<Base_class>::RegisterObject
(
	const std::string Object_ID,
	const std::string & Object_name, 
	IOFactory<Base_class>::DerivedClassCreator CreateThisObject
)
{
	CreateObjectsMap_[Object_name] = CreateThisObject;
	IDLookUpMap_[Object_ID] = Object_name;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  CreateObject
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> Base_class * IOFactory<Base_class>::CreateObject()
{
	ConfigurationManager & con = ConfigurationManager::Instance();
	std::string o_type = con.GetValue<std::string>(IdentifierType(ClassName<Base_class>()));
	
	typename LookUpMap::const_iterator i = IDLookUpMap_.find(o_type);
	if (i == IDLookUpMap_.end())
	{
		throw std::runtime_error("IOFactory:  ID " + o_type + " not found");
	}

	std::string nm = i->second;
	DoMonitorOutput(nm);

	typename CreateMap::const_iterator j = CreateObjectsMap_.find(nm);
	if (j == CreateObjectsMap_.end())
	{
		throw std::runtime_error("IOFactory:  Name " + nm + " not found");
	}

	return j->second();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  DoMonitorOutput
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> void IOFactory<Base_class>::DoMonitorOutput(std::string & nm)
{
	if (mon_ == 0) mon_ = ConfigurationManager::Instance().GetMonitor();
	mon_->OutputIOspecs(ClassName<Base_class>() + " is " + nm);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
