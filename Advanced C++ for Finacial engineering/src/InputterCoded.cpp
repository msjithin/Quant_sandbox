//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	InputterCoded.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <map>
#include <string>

#include "InputterCoded.h"
#include "MonitorManager.h"

#include "IOFactoryRegistration.h"
#include "CreateableBase.h"

#include "ParameterBase.h"

#include "ParameterS0.h"
#include "ParameterR.h"
#include "ParameterSigma.h"

#include "ParameterM.h"
#include "ParameterN.h"
#include "ParameterTmax.h"

#include "ParameterLambda.h"
#include "ParameterEta.h"
#include "ParameterNu.h"
#include "ParameterKappa.h"

#include "ParameterPrune.h"

class ApplicationBase;
class BranchesBase;
class ProcessBase;
class SliceBase;
class ValuationMethodBase;
class AccumulatorBase;
class WienerBase;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  createable
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<InputterCoded>()
{
    return "InputterCoded";
}

template<> std::string ClassID<InputterCoded>()
{
    return "c";
}

template<> std::string BaseClassName<InputterCoded>()
{
    return ClassName<InputterBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	IOFactoryRegistration<InputterBase, InputterCoded> RegisterInputterCoded;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	interface
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

InputterCoded::InputterCoded()
{
	mon_->OutputLine("Input is hard wired");

    sd_map_.clear();
   	sd_map_[ParameterS0().name] = 100;
   	sd_map_[ParameterR().name] = 0.05;
   	sd_map_[ParameterSigma().name] = 0.20;
   	
   	sd_map_[ParameterKappa().name] = 0.0;  // Lattice parameter.  0 mean use critical value

   	sd_map_[ParameterM().name] = 100000;
   	sd_map_[ParameterN().name] = 100;
   	sd_map_[ParameterTmax().name] = 1;
   	
   	sd_map_[ParameterLambda().name] = 0.05;
   	sd_map_[ParameterEta().name] = 1;
   	sd_map_[ParameterNu().name] = 0.20;
   	
   	sd_map_[ParameterPrune().name] = 8.0;				// lattice pruning parameter

   	ss_map_.clear();
   	ss_map_[ClassName<ApplicationBase>()] = "v";    	// v for valulation
    ss_map_[ClassName<ProcessBase>()] = "g";    		// g for gbm,  m for merton
    ss_map_[ClassName<WienerBase>()] = "s";    			// p for plain,  a for antithetic, s for stratified
    ss_map_[ClassName<AccumulatorBase>()] = "p";    	// p for plain accumulator
   	ss_map_[ClassName<ValuationMethodBase>()] = "l";   	// p for plain MC,  l for lattice
    ss_map_[ClassName<BranchesBase>()] = "3";    		// 3 for trinmial,  7 for heptanomial
    ss_map_[ClassName<SliceBase>()] = "m";  			// m for multiplicative, p for pruned
}
  
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
