//***************************************************************************
//  IOFactoryRegistration.cpp
//***************************************************************************

#ifndef IOFactoryRegistrationH
#define IOFactoryRegistrationH

//---------------------------------------------------------------------------

class Environment;

//***************************************************************************
//	template <...> class IOFactoryRegistration
//***************************************************************************

template<typename Base_class, typename Derived_class> class IOFactoryRegistration
{
	public:
	    IOFactoryRegistration();

		static Base_class * Create();
};

//***************************************************************************
//  Must (i)  #include the .cpp in the .h
//       (ii) not add the .cpp to the project
//  for template classes
//***************************************************************************

#include "IOFactoryRegistration.cpp"

//***************************************************************************
//  end of file
//***************************************************************************

#endif
