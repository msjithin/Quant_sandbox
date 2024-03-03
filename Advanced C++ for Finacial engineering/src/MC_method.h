//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  MC_method.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef MC_methodH
#define MC_methodH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <vector>

#include "ValuationMethodBase.h"

class OutputManager;
class OptionOutputManager;
class MonitorManager;

class OptionBook;
class ProcessBase;
class AccumulatorBook;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class MC_method
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class MC_method : public ValuationMethodBase
{
    public:
        explicit MC_method();
        ~MC_method();
        
        void run(OptionBook & opt, ProcessBase & gbm);
        void SetOutput(OutputManager &) const;
        void SetOutput(OptionOutputManager &) const;

	   	// CreatableBase stuff
	   	std::string GetName() const;
	   	std::string GetID() const;
	   	std::string GetBaseClassName() const;
	   	std::string GetStringify() const;

    private:
        long M_;
        long N_;
        
        double Tmax_;
        
        long interval_;
        
    	std::vector<double> path_;

        AccumulatorBook * acc_;
        MonitorManager * mon_;
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
