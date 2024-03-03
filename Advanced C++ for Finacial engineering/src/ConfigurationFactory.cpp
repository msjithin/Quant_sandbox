//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  ConfigurationFactory.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>
#include <map>
#include <stdexcept>

#include "SetUpManager.h"
#include "CreateableBase.h"

#include "IdentifierType.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> ConfigurationFactory<Base_class>::ConfigurationFactory()
{
	CreateObjectsMap_.clear();
	IDLookUpMap_.clear();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Register with Factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<typename Base_class> void ConfigurationFactory<Base_class>::RegisterObject
(
	const std::string Object_ID,
	const std::string & Object_name, 
	ConfigurationFactory<Base_class>::DerivedClassCreator CreateThisObject
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

template<typename Base_class> Base_class * ConfigurationFactory<Base_class>::CreateObject()
{
	SetUpManager & set = SetUpManager::Instance();
	std::string o_type = set.GetValue<std::string>(IdentifierType(ClassName<Base_class>()));

	typename LookUpMap::const_iterator i = IDLookUpMap_.find(o_type);
	if (i == IDLookUpMap_.end())
	{
		throw std::runtime_error("ConfigurationFactory:  ID " + o_type + " not found");
	}

	std::string nm = i->second;
	typename CreateMap::const_iterator j = CreateObjectsMap_.find(nm);
	if (j == CreateObjectsMap_.end())
	{
		throw std::runtime_error("ConfigurationFactory:  Name " + nm + " not found");
	}

	return j->second();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

