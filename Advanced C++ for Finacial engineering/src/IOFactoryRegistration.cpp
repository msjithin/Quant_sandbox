//***************************************************************************
//  IOFactoryRegistration.cpp
//***************************************************************************

#include <string>

#include "IOFactory.h"

#include "CreateableBase.h"

//***************************************************************************
//	IOFactoryRegistration constructor
//***************************************************************************

template<typename Base_class, typename Derived_class>
IOFactoryRegistration<Base_class, Derived_class>::IOFactoryRegistration()
{
    IOFactory<Base_class>::Instance().RegisterObject
    (
        ClassID<Derived_class>(),
        ClassName<Derived_class>(),
        IOFactoryRegistration<Base_class, Derived_class>::Create
    );
}

//***************************************************************************
//	Registered function
//***************************************************************************

template<typename Base_class, typename Derived_class>
Base_class * IOFactoryRegistration<Base_class, Derived_class>::Create()
{
    return new Derived_class();
}

//***************************************************************************
//  end of file
//***************************************************************************


