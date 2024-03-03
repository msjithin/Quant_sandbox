//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  SliceBook.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef SliceBookH
#define SliceBookH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <vector>

class BranchesBase;
class OptionBook;
class SliceBase;
class ProcessBase;
class VecB;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  SliceBook
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class SliceBook
{
	public:
		SliceBook();
		~SliceBook();

        void SetBranches(BranchesBase &);
        void SetValues(const OptionBook &, const ProcessBase &);
        void SetSvalues(VecB & Svals);
       
        void DoOneStep(VecB & S_vals, long j,  double c_time);

        void GetOptionIDs(VecB &);
        void GetValues(VecB &);
        long GetBound(){return bound_;}

	private:
		SliceBook(const SliceBook &);             // Disable
		SliceBook & operator=(const SliceBook &);

		std::vector<SliceBase *> slc_;		      // Care.  vector of pointers
		
		BranchesBase * brch_;

		long Nopts_;                              // number of options
		
		long bound_;							  // array bound
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

 
