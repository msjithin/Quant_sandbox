//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  OptionAverageRate.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef OptionAverageRateH
#define OptionAverageRateH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <vector>
#include "OptionBase.h"

class VecB;
class Parameters;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class OptionAverageRate
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class OptionAverageRate : public OptionBase
{
    public:
        explicit OptionAverageRate(const Parameters &);
        
        void initialize(double, long);
       
		double GetT() const {return T_;}
        long GetOptionID() const {return OptionID_;}

        double ComputePayoffPath(const std::vector<double> &) const;
        double ComputePayoff(const double) const;
        void ComputePayoffSlice(const double, const VecB &, VecB &) const;

        bool IsEarlyExercise() const {return early_;}
        bool IsExerciseTime(double t) const;

	   // CreatableBase stuff
	   std::string GetName() const;
	   std::string GetID() const;
	   std::string GetBaseClassName() const;
	   std::string GetStringify() const;

    private:
		long OptionID_;

        double X_;
        double T_;

        long Tindex_;

		bool early_;

        double ComputePO(double S) const;
		
		void SetTindex(double, long);
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
