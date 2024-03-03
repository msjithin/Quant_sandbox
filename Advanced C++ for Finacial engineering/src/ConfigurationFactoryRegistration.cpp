//***************************************************************************
//  ConfigurationFactoryRegistration.cpp
//***************************************************************************

#include <string>
//#include <iostream>

#include "ConfigurationFactory.h"

#include "CreateableBase.h"

//***************************************************************************
//	ConfigurationFactoryRegistration constructor
//***************************************************************************

template<typename Base_class, typename Derived_class>
ConfigurationFactoryRegistration<Base_class, Derived_class>::ConfigurationFactoryRegistration()
{
    ConfigurationFactory<Base_class>::Instance().RegisterObject
    (
        ClassID<Derived_class>(),
        ClassName<Derived_class>(),
        ConfigurationFactoryRegistration<Base_class, Derived_class>::Create
    );
}

//***************************************************************************
//	Registered function
//***************************************************************************

template<typename Base_class, typename Derived_class>
Base_class * ConfigurationFactoryRegistration<Base_class, Derived_class>::Create()
{
    return new Derived_class;
}

//***************************************************************************
//  end of file
//***************************************************************************


