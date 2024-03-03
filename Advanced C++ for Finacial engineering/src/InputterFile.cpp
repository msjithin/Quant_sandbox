//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  InputterFile.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <stdexcept>
#include <string>
#include <vector>

#include "utility.h"
#include "lib_val.h"
#include "InputterFile.h"
#include "MonitorManager.h"

#include "ParameterInputFilePath.h"

#include "IOFactoryRegistration.h"
#include "ConfigurationManager.h"
#include "CreateableBase.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<InputterFile>()
{
    return "InputterFile";
}

template<> std::string ClassID<InputterFile>()
{
    return "f";
}

template<> std::string BaseClassName<InputterFile>()
{
    return ClassName<InputterBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	IOFactoryRegistration<InputterBase, InputterFile> RegisterInputterFile;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

InputterFile::InputterFile()
:   Data_Fetched_(false)
{
	input_file_path_ = ConfigurationManager::Instance().GetValue<std::string>(ParameterInputFilePath());
	
	mon_->OutputLine(lv::join("Input is from file ", input_file_path_));

    i_stream_.open(input_file_path_.c_str(), std::ios::in);
	if (!i_stream_.is_open())
	{
	    throw std::runtime_error("InputterFile:  cannot open input file");
	}

    FetchData();
	WriteMaps();
}

InputterFile::~InputterFile()
{
    i_stream_.close();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  FetchData().  Very restrictive.  Up to the client to make the file correspond to this
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void InputterFile::FetchData()
{
    if (Data_Fetched_) return;
	
    sc_map_.clear();
    sd_map_.clear();
    ss_map_.clear();

	sd_map_.insert(FetchPair<double>());        // S0
    sd_map_.insert(FetchPair<double>());        // r
    sd_map_.insert(FetchPair<double>());        // sigma

    sd_map_.insert(FetchPair<double>());        // kappa

    sd_map_.insert(FetchPair<double>());        // N
    sd_map_.insert(FetchPair<double>());        // M
    sd_map_.insert(FetchPair<double>());        // Tmax

    sd_map_.insert(FetchPair<double>());        // lambda
    sd_map_.insert(FetchPair<double>());        // eta
    sd_map_.insert(FetchPair<double>());        // nu
    
    sd_map_.insert(FetchPair<double>());        // prune

    ss_map_.insert(FetchPair<std::string>());   // Application Type
    ss_map_.insert(FetchPair<std::string>());   // Process Type
    ss_map_.insert(FetchPair<std::string>());   // Wiener Type
    ss_map_.insert(FetchPair<std::string>());   // Method Type

    ss_map_.insert(FetchPair<std::string>());   // Accumulator Type
    ss_map_.insert(FetchPair<std::string>());   // Branching Type
    ss_map_.insert(FetchPair<std::string>());   // Slice Type

	Data_Fetched_ = true;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	utilities
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void InputterFile::WriteMaps()
{
    ut::OutputLine("The following data has been input from file:");
	WriteSDmap();
    ut::OutputLine();
    WriteSCmap();
    ut::OutputLine();
    WriteSSmap();
    ut::OutputLine();
}

void InputterFile::WriteSDmap()
{
     SD_it it_end = sd_map_.end();
     for(SD_it it = sd_map_.begin(); it != it_end;  ++it )
     {
         ut::OutputLine(it->first, it->second);
     }
}

void InputterFile::WriteSCmap()
{
     SC_it it_end = sc_map_.end();
     for(SC_it it = sc_map_.begin(); it != it_end;  ++it )
     {
         ut::OutputLine(it->first, char(it->second));
     }
}

void InputterFile::WriteSSmap()
{
     SS_it it_end = ss_map_.end();
     for(SS_it it = ss_map_.begin(); it != it_end;  ++it )
     {
         ut::OutputLine(it->first, it->second);
     }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

