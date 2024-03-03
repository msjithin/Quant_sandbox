//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	SliceMultiplicative.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef SliceMultiplicativeH
#define SliceMultiplicativeH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "VecB.h"

#include "SliceBase.h"

class OptionBase;
class ProcessBase;
class BranchesBase;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Slice
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class SliceMultiplicative : public SliceBase
{
    public:
        explicit SliceMultiplicative();
        ~SliceMultiplicative();
       
        void SetValues(const OptionBase & opt, const ProcessBase & proc);
        void SetBranches(const BranchesBase & ptb);
        void SetSvalues(VecB & Svals);
        
        long GetBound() const {return ub_max_;}

        void DoOneStep(VecB & S_vals, long j,  double c_time);

        double GetOValue() const {return (*this_value_)[0];}
        long GetOptionID() const {return ID_;}
       
	    // CreatableBase stuff
	    std::string GetName() const;
	    std::string GetID() const;
	    std::string GetBaseClassName() const;
	    std::string GetStringify() const;

    private:
        const OptionBase * opt_;        // Does not own
        const ProcessBase * prc_;       // Does not own
        const BranchesBase * prb_;      // Does not own
        
        double T_;						// option maturity time
        long NT_;						// option maturity time step
        long ID_;						// option ID number
        bool Early_exercise_;
        
        VecB * this_value_;  // pointers so can swap cheaply
        VecB * next_value_;
        
        VecB * S_values_;
        VecB payoffs_;

        long N_;                  // time steps
        double Tmax_;             // max time on the lattice
        double dis_dt_;           // discount factor over time dt
        
        long upbranches_;         // # of up-branches
        long ub_max_;         	  // upper array bound 

    	double current_t_;   	  // current value of time
    	double dt_;   	  		  // time step

        void roll_back(long j);     // rolls back option values (European)
        void Early_update(VecB & S_vals, long j, double t);  // updates for early exercise
        void roll_over(long j);     // rolls over to the next time

        void Bermudan_update(long j);

		void SetNT();            // checks option payoff is whole number of time steps
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX





