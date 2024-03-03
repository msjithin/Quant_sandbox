//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionInputDefault.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>

#include "OptionInputDefault.h"

#include "IO_Manager.h"
#include "IOFactoryRegistration.h"
#include "Parameters.h"

#include "ParameterX.h"
#include "ParameterT.h"
#include "ParameterOptionID.h"

#include "CreateableBase.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXvXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  CreateableBase
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<OptionInputDefault>()
{
    static std::string name = "OptionInputDefault";
    return name;
}

template<> std::string ClassID<OptionInputDefault>()
{
    static std::string name = "c";
    return name;
}

template<> std::string BaseClassName<OptionInputDefault>()
{
    return ClassName<OptionInputBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	IOFactoryRegistration<OptionInputBase, OptionInputDefault> RegisterOptionInputDefault;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionInputDefault::OptionInputDefault()
{
    SetSpecs();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Getters()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

long OptionInputDefault::GetNOptions() const
{
    return specs_.size();
}

std::string OptionInputDefault::GetType(const long i)
{
	return specs_[i].first;
}

Parameters OptionInputDefault::GetParas(const long i)
{
    Parameters paras;
    paras.SetValues(specs_[i].second);
    return paras;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetSpecs()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void OptionInputDefault::SetSpecs()
{
	std::string opt_type = "c";  		// c for European call,  p for European put, a for average rate
	                  					// C for American call,  P for American put
    double X = 100;    					// X
    double T = 1;    					// T
    long OptionID = 23;                 // Option ID

  	ParamMap pmap;
	pmap.insert(ParamPair(ParameterX().name, X));
	pmap.insert(ParamPair(ParameterT().name, T));
	pmap.insert(ParamPair(ParameterOptionID().name, OptionID));

    OptionSpec ospec;
	ospec.first = opt_type;
  	ospec.second = pmap;

	specs_.push_back(ospec);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreateableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string OptionInputDefault::GetName() const {return ClassName<OptionInputDefault>();}
std::string OptionInputDefault::GetID() const {return ClassID<OptionInputDefault>();}
std::string OptionInputDefault::GetBaseClassName() const {return BaseClassName<OptionInputDefault>();}
std::string OptionInputDefault::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

