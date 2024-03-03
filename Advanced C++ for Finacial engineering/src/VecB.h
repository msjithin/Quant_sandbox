//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   VecB.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef VecB_H
#define VecB_H
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <iostream>
#include <algorithm>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   Class VecB.   Vecor bounded
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class VecB
{
	public:
		VecB();                             // Default constructor, null vector
	   	explicit VecB(const long N);        // N elements, 1 to N, initialised to zero
	   	VecB(const long lb, const long ub); // lb to ub elements initialised to zero
	   	VecB(const long lb, const long ub, const double a);	// initialised to a

		//XXXXX rule of three methods XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		~VecB();                            // non-virtual,  so cannot be a base class
		VecB(const VecB &);
		VecB & operator=(const VecB &);
		 		
		void swap(VecB &);
		VecB * clone() const;
		
		//XXXXX indexing operators XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		const double & operator[](const long) const;          // Indexing (r-value)
		double & operator[](const long);                      // Indexing (l-value)
		const double & at(const long) const;                  // returns an r-value
		
		double front() const;                                 // returns the first element
		double back() const;                                  // returns the last element

		//XXXXX overloaded arithmetic operators XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		friend VecB operator+(const VecB &, const VecB &);    // Vector addition
	   	friend VecB operator-(const VecB &, const VecB &);    // Vector subtraction
	   	friend VecB operator*(const VecB &, const VecB &);    // Vector multiplication
	   	friend VecB operator/(const VecB &, const VecB &);    // Vector division

		friend VecB operator+(const double, const VecB &);    // Scalar addition
		friend VecB operator+(const VecB &, const double);    // Scalar addition
		friend VecB operator*(const double, const VecB &);    // Scalar multiplication
		friend VecB operator*(const VecB &, const double);    // Scalar multiplication

		friend VecB operator-(const VecB &);                  // monadic sign change

		//XXXXX overloaded insertion operator XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

	   	friend std::ostream & operator<<(std::ostream &, const VecB &); // Output for VecB

		//XXXXX pop and push methods XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		void push_back(const double);                         // push back an extra element
		void push_front(const double);                        // push front an extra element
		double pop_front();                         		  // pop element from front
		double pop_back();                         			  // pop element from back

		//XXXXX size methods XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		long LBound() const {return lb_;}                     // getters
		long UBound() const {return ub_;}
		long size() const {return N_;}

		void resize(long ub);                                 // re-size array bounds.
		void resize(long lb, long ub);                        // re-initializes
		void resize_preserve(long lb, long ub);               // preserves data

		double norm() const;                                  //vector norm

	private:
		double * x_;                   // The data
		long N_;                       // Its length
		
		long lb_;                      // The lower index bound
		long ub_;                      // The upper index bound

		double * clone_data() const;   // Creates a new copy of x_
		void CheckIndex(long i) const; // Checks i is a valid index
};

//XXXXXXXXXXXXXXXX declaration of overloaded std::swap XXXXXXXXXXXXXXXXXXXXXXXXX

namespace std  
{
	template<> void swap<VecB>(VecB & a, VecB & b);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
