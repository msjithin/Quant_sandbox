//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SliceMultiplicativePrune.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef SliceMultiplicativePruneH
#define SliceMultiplicativePruneH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "VecB.h"

#include "SliceBase.h"

class OutputManager;
class Monitor;

class OptionBase;
class ProcessBase;
class BranchesBase;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Slice
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
       
class SliceMultiplicativePrune : public SliceBase
{
    public:
        explicit SliceMultiplicativePrune();
        ~SliceMultiplicativePrune();
       
        void SetValues(const OptionBase & opt, const ProcessBase & proc);
        void SetBranches(const BranchesBase & ptb);
        void SetSvalues(VecB &);

        long GetBound() const {return ub_cur_;}

        void DoOneStep(VecB & S_vals, long j,  double c_time);

        double GetOValue() const {return (*this_value_)[0];}
        long GetOptionID() const {return ID_;}

		// CreatableBase declarations
		std::string GetName() const;
		std::string GetID() const;
		std::string GetBaseClassName() const;
		std::string GetStringify() const;

    private:           
        const OptionBase * opt_;        // Does not own
        const ProcessBase * prc_;       // Does not own
        const BranchesBase * prb_;      // Does not own
        
        double T_;						// option maturity time
        double NT_;						// option maturity time step
        long ID_;						// option ID number
        bool Early_exercise_;
        
        VecB * this_value_;  // pointers so can swap cheaply
        VecB * next_value_;
        
        VecB * S_values_;
        VecB payoffs_;

        long N_;                  // time steps
        double Tmax_;             // max time on the lattice
        double dis_dt_;           // discount factor over time dt
        
        double lambda_;           // pruning parameter,  sds.
        long N_prune_;            // pruning level,  steps
        long j_prune_;            // pruning level,  upbranches_

        long upbranches_;         // # of up-branches
        long ub_cur_;         	  // current uppe bound of array

    	double current_t_;   	  // current value of time
    	double dt_;   	  		  // time step

        void roll_back(long j);     // rolls back option values (European)
        void Early_update(VecB &, long, double);  // updates for early exercise
        void roll_over(long j);     // rolls over to the next time

        void Bermudan_update(long j);
		void SetNT();
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX





