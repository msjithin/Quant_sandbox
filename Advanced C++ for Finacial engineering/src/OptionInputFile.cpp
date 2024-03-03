//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionInputFile.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "lib_val.h"

#include "ConfigurationManager.h"
#include "IOFactoryRegistration.h"
#include "OptionInputFile.h"
#include "Parameters.h"

#include "ParameterOptionInputFilePath.h"

#include "CreateableBase.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  CreateableBase
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<OptionInputFile>()
{
    static std::string name = "OptionInputFile";
    return name;
}

template<> std::string ClassID<OptionInputFile>()
{
    static std::string name = "f";
    return name;
}

template<> std::string BaseClassName<OptionInputFile>()
{
    return ClassName<OptionInputBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	IOFactoryRegistration<OptionInputBase, OptionInputFile> RegisterOptionInputFile;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionInputFile::OptionInputFile()
{
	ConfigurationManager & io = ConfigurationManager::Instance();
	input_file_path_ = io.GetValue<std::string>(ParameterOptionInputFilePath());

    i_stream_.open(input_file_path_.c_str(), std::ios::in);
	if (!i_stream_.is_open())
	{
		throw std::runtime_error("OptionInputFile:  cannot open input file");
	}
	
	specs_.resize(0);
    SetSpecs();
}

OptionInputFile::~OptionInputFile()
{
    i_stream_.close();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Getters()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

long OptionInputFile::GetNOptions() const
{
    return specs_.size();
}

std::string OptionInputFile::GetType(const long i)
{
	return specs_[i].first;
}

Parameters OptionInputFile::GetParas(const long i)
{
    Parameters paras;
    paras.SetValues(specs_[i].second);
    return paras;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SetSpecs()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void OptionInputFile::SetSpecs()
{
	std::string it1;   //first item in row
	std::string it2;   //second item in row

    while (i_stream_ >> it1)
    {
	    i_stream_ >> it2;
	    
	    if (it1 != "Begin") throw std::runtime_error("OptionInputFile::SetSpecs:  bad input");

		OptionSpec ospec;
		ospec.first = it2;

  		ParamMap pmap;
		ReadInSpec(pmap);
		 	
  		ospec.second = pmap;
	 	    
		specs_.push_back(ospec);
    }
    i_stream_ >> it1;
}

void OptionInputFile::ReadInSpec(ParamMap & pmap)
{
	std::string i1;   //first item in row
	std::string i2;   //second item in row
	
	bool KeepGoing = true;
	
	while (KeepGoing)
	{
	    i_stream_ >> i1 >> i2;

		if (i1 == "Begin" || i2 == "Begin" ||  i2 == "End")
	        throw std::runtime_error("OptionInputFile::ReadInSpec:  bad input");

		if (i1 == "End") KeepGoing = false;
	    else
	    {
		 	double d2 = lv::StringToDouble(i2);
			pmap.insert(ParamPair(i1, d2));
		}
    }

	if (pmap.size() > 0) return;
	else throw std::runtime_error("OptionInputFile::ReadInSpec:  no params");
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreateableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string OptionInputFile::GetName() const {return ClassName<OptionInputFile>();}
std::string OptionInputFile::GetID() const {return ClassID<OptionInputFile>();}
std::string OptionInputFile::GetBaseClassName() const {return BaseClassName<OptionInputFile>();}
std::string OptionInputFile::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  input file path =  " + input_file_path_ + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

