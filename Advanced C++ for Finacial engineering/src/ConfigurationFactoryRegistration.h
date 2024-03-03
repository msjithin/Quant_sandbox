//***************************************************************************
//  ConfigurationFactoryRegistration.cpp
//***************************************************************************

#ifndef ConfigurationFactoryRegistrationH
#define ConfigurationFactoryRegistrationH

//***************************************************************************
//	template <...> class ConfigurationFactoryRegistration
//***************************************************************************

template<typename Base_class, typename Derived_class> class ConfigurationFactoryRegistration
{
	public:
	    ConfigurationFactoryRegistration();

		static Base_class * Create();
};

//***************************************************************************
//  Must (i)  #include the .cpp in the .h
//       (ii) not add the .cpp to the project
//  for template classes
//***************************************************************************

#include "ConfigurationFactoryRegistration.cpp"

//***************************************************************************
//  end of file
//***************************************************************************

#endif
