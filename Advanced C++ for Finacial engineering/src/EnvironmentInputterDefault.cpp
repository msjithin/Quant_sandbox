//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	EnvironmentInputterDefault.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <stdexcept>

#include "EnvironmentInputterDefault.h"

#include "ConfigurationFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterInterval.h"
#include "ParameterOutputFilePath.h"
#include "ParameterInputFilePath.h"
#include "ParameterOptionInputFilePath.h"
#include "ParameterOptionOutputFilePath.h"

class InputterBase;
class OutputterBase;

class OptionInputBase;
class OptionOutputBase;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<EnvironmentInputterDefault>()
{
    static std::string name = "EnvironmentInputterDefault";
    return name;
}

template<> std::string ClassID<EnvironmentInputterDefault>()
{
    static std::string name = "d";
    return name;
}

template<> std::string BaseClassName<EnvironmentInputterDefault>()
{
    return ClassName<EnvironmentInputterBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	ConfigurationFactoryRegistration<EnvironmentInputterBase, EnvironmentInputterDefault> RegisterEID;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

EnvironmentInputterDefault::EnvironmentInputterDefault()
{
    sd_map_.clear();
   	sc_map_.clear();
    ss_map_.clear();

   	ss_map_[ParameterInputFilePath().name] = "Input_data.txt";	 	// relative path
   	ss_map_[ParameterOutputFilePath().name] = "Output_data.txt";	// relative path

   	ss_map_[ParameterOptionInputFilePath().name] = "Option_input_data.txt";	 	// relative path
   	ss_map_[ParameterOptionOutputFilePath().name] = "Option_output_data.txt";	// relative path

   	ss_map_[ClassName<InputterBase>()] = "c";     					// c for code, f for file
   	ss_map_[ClassName<OutputterBase>()] = "c";    					// c for console, f for file
   	
   	ss_map_[ClassName<OptionInputBase>()] = "f";     				// c for code, f for file
   	ss_map_[ClassName<OptionOutputBase>()] = "c";    				// c for console, f for file
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
