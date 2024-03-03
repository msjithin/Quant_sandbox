//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  OptionOutputFile.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>
#include <fstream>

#include "OptionOutputFile.h"

#include "ParameterOptionOutputFilePath.h"

#include "IOFactoryRegistration.h"
#include "CreateableBase.h"
#include "ConfigurationManager.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  CreateableBase
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<OptionOutputFile>()
{
    static std::string name = "OptionOutputFile";
    return name;
}

template<> std::string ClassID<OptionOutputFile>()
{
    static std::string name = "f";
    return name;
}

template<> std::string BaseClassName<OptionOutputFile>()
{
    return ClassName<OptionOutputBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	IOFactoryRegistration<OptionOutputBase, OptionOutputFile> RegisterOptionOutputFile;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionOutputFile::OptionOutputFile()
:   stream_precision_(15)    	//set stream precision,  as you wish
,   stream_width_(15) 			//set stream width,  as you wish
{
	ConfigurationManager & io = ConfigurationManager::Instance();
	output_file_path_ = io.GetValue<std::string>(ParameterOptionOutputFilePath());
    
    //over-write file if exists
    o_stream_.open(output_file_path_.c_str(), std::ios::out | std::ios::trunc);
    
	o_stream_.precision(stream_precision_);
	o_stream_.width(stream_width_);
}

OptionOutputFile::~OptionOutputFile()
{
    o_stream_.close();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  putters
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void OptionOutputFile::OutputDouble(const std::string & str, const double val)
{
     o_stream_ << str << val << std::endl;
}
 
void OptionOutputFile::OutputString(const std::string & str)
{
     o_stream_ << str << std::endl;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreateableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string OptionOutputFile::GetName() const {return ClassName<OptionOutputFile>();}
std::string OptionOutputFile::GetID() const {return ClassID<OptionOutputFile>();}
std::string OptionOutputFile::GetBaseClassName() const {return BaseClassName<OptionOutputFile>();}
std::string OptionOutputFile::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  output file path =  " + output_file_path_ + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

