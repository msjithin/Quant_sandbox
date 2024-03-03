//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	LatticeMethod.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef LatticeMethodH
#define LatticeMethodH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <vector>

#include "ValuationMethodBase.h"

class OutputManager;
class OptionOutputManager;
class MonitorManager;

class OptionBook;
class ProcessBase;

class BranchesBase;
class SliceBook;
class VecB;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	LatticeMethod
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class LatticeMethod : public ValuationMethodBase
{
    public:
        explicit LatticeMethod();
        ~LatticeMethod();
       
        void run(OptionBook & opt, ProcessBase & gbm);
        void SetOutput(OutputManager &) const;
        void SetOutput(OptionOutputManager &) const;

	   // CreatableBase stuff
	   std::string GetName() const;
	   std::string GetID() const;
	   std::string GetBaseClassName() const;
	   std::string GetStringify() const;

    private:
		MonitorManager * mon_;    // does not own
        SliceBook * slc_;         // Owns

        VecB * S_values_;         // LatticeMethod owns S_values & rolls back

        BranchesBase * prb_;      // Owns
        
        long N_;                  // time steps
        double Tmax_;             // final time
        double dt_;               // time step
        double current_t_;        // current time

        double scale_s_;          // scaling factor for S-rollback
        
        long interval_;           // counter step interval

		void ComputeSvalues(ProcessBase & proc);	  // Computes S_values_ for time Tmax_
		void roll_back_S(long j); // computes S values for the next time step
        void evolve_back(long);
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX





