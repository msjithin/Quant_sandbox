//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   VecB.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <algorithm>

#include <sstream>

#include "VecB.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   basic overloads                                                                            
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace std
{
	template<> void swap<VecB>(VecB & a, VecB & b)
	{
		a.swap(b);
	}
}

std::ostream & operator<<(std::ostream & out, const VecB & a)
{
	out << "( ";
	if (a.N_ >= 1)
	{
		out << a.x_[0];
		for(long i = 1; i != a.N_; ++i) out << ", " << a.x_[i];
	}
	return out << ")";
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   constructors stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

VecB::VecB()
:	x_(0)
,	N_(0)
,	lb_(0)
,	ub_(0)
{}
			
VecB::VecB(const long N)  // bounds 1 to N,  N elements, initialized to 0
:	x_(0)
,	N_(N)
,	lb_(1)
,	ub_(N)
{
	if (lb_ > ub_) throw std::runtime_error("VecN: index bounds error");
	x_= new double[N_];
	for(long i = 0; i != N_; ++i) x_[i] = 0.0;
}

VecB::VecB(const long lb, const long ub)	//lb to ub elements initialised to 0
:	x_(0)
,	lb_(lb)
,	ub_(ub)
{
	if (lb_ > ub_) throw std::runtime_error("VecN: index bounds error");
	N_ = 1 + ub_ - lb_;
	
	x_= new double[N_];
	for(long i = 0; i != N_; ++i) x_[i] = 0.0;
}

VecB::VecB(const long lb, const long ub, const double a)	//lb to ub elements initialised to a
:	x_(0)
,	lb_(lb)
,	ub_(ub)
{
	if (lb_ > ub_) throw std::runtime_error("VecN: index bounds error");
	N_ = 1 + ub_ - lb_;

	x_= new double[N_];
	for(long i = 0; i != N_; ++i) x_[i] = a;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   rule of three
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

VecB::~VecB()
{
	delete [] x_;
}

VecB::VecB(const VecB & original)
:	x_(0)	//Initialized, so can delete harmlessly
{
	try
	{
		if (original.x_ != 0) x_ = original.clone_data();
	}
	catch(...)
	{
		delete [] x_;
		throw;
	}

	N_ = original.N_;
	lb_ = original.lb_;
	ub_ = original.ub_;
}

VecB & VecB::operator=(const VecB & original)
{
	VecB temp(original);
	swap(temp);          
	return *this;
}

void VecB::swap(VecB & other)
{
    using std::swap;
	swap(x_, other.x_);		
	swap(N_, other.N_);
	swap(ub_, other.ub_);
	swap(lb_, other.lb_);
}

VecB * VecB::clone() const
{
	return new VecB(*this);
}

double * VecB::clone_data() const
{
	double * new_x = new double[N_];
	for(long i = 0; i != N_; ++i) new_x[i] = x_[i];
	return new_x;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   overloaded operators and methods,  indexing
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

const double & VecB::operator[](const long i) const  //r-value
{
	CheckIndex(i);
	return x_[i - lb_];
}
			
double & VecB::operator[](const long i)              // l-value
{
	CheckIndex(i);
	return x_[i - lb_];
}						

const double & VecB::at(const long i) const          //r-value
{
	CheckIndex(i);
	return x_[i - lb_];
}

double VecB::front() const {return x_[0];}
double VecB::back() const {return x_[N_-1];}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   overloaded operators,  arithmetic
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

VecB operator+(const VecB & lhs, const VecB & rhs)
{
	if ((lhs.lb_ != rhs.lb_) || (lhs.ub_ != rhs.ub_)) throw std::runtime_error("VecB: bounds mismatch");

	VecB result(lhs);
	long N = result.N_;
	for(long i = 0; i != N; ++i) result.x_[i] += rhs.x_[i];
	return result;
}

VecB operator-(const VecB & lhs, const VecB & rhs)
{
	if ((lhs.lb_ != rhs.lb_) || (lhs.ub_ != rhs.ub_)) throw std::runtime_error("VecB: bounds mismatch");

	VecB result(lhs);
	long N = result.N_;
	for(long i = 0; i != N; ++i) result.x_[i] -= rhs.x_[i];
	return result;
}

VecB operator*(const VecB & lhs, const VecB & rhs)
{
	if ((lhs.lb_ != rhs.lb_) || (lhs.ub_ != rhs.ub_)) throw std::runtime_error("VecB: bounds mismatch");

	VecB result(lhs);
	long N = result.N_;
	for(long i = 0; i != N; ++i) result.x_[i] *= rhs.x_[i];
	return result;
}

VecB operator/(const VecB & lhs, const VecB & rhs)
{
	if ((lhs.lb_ != rhs.lb_) || (lhs.ub_ != rhs.ub_)) throw std::runtime_error("VecB: bounds mismatch");

	VecB result(lhs);
	long N = result.N_;
	for(long i = 0; i != N; ++i) result.x_[i] /= rhs.x_[i];
	return result;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   scalar operators
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

VecB operator+(const double a, const VecB & rhs)
{
	VecB result(rhs);
	long N = result.N_;
	for(long i = 0; i != N; ++i) result.x_[i] += a;
	return result;
}

VecB operator+(const VecB & lhs, const double a)
{
	VecB result(lhs);
	long N = result.N_;
	for(long i = 0; i != N; ++i) result.x_[i] += a;
	return result;
}

VecB operator*(const double a, const VecB & rhs)
{
	VecB result(rhs);
	long N = result.N_;
	for(long i = 0; i != N; ++i) result.x_[i] *= a;
	return result;
}

VecB operator*(const VecB & lhs, const double a)
{
	VecB result(lhs);
	long N = result.N_;
	for(long i = 0; i != N; ++i) result.x_[i] *= a;
	return result;
}

VecB operator-(const VecB & lhs)
{
	VecB result(lhs);
	long N = result.N_;
	for(long i = 0; i != N; ++i) result.x_[i] = -result.x_[i];
	return result;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   misc operators
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double VecB::norm() const
{
	double sum = 0.0;
	for(long i = 0; i != N_; ++i) sum += x_[i]*x_[i];
	return std::sqrt(sum);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   pop and push methods
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void VecB::push_back(const double a)
{
	double * temp = new double[N_ + 1];
	for(long i = 0; i != N_; ++i) temp[i] = x_[i];
	temp[N_] = a;

	std::swap(temp, x_);
	delete [] temp;
	++N_;
	++ub_;  //increases ub by one
}

void VecB::push_front(const double a)
{
	double * temp = new double[N_ + 1];
	temp[0] = a;
	for(long i = 0; i != N_; ++i) temp[i+1] = x_[i];

	std::swap(temp, x_);
	delete [] temp;
	++N_;
	--lb_;   //decreases lb by one.
}

double VecB::pop_front()
{
	if (N_ == 0) throw std::runtime_error("VecB: nothing to pop");
	double a = x_[0];
	
	double * temp = new double[N_ - 1];
	for(long i = 1; i != N_; ++i) temp[i-1] = x_[i];

	std::swap(temp, x_);
	delete [] temp;
	--N_;
	++lb_; // increases lb by one
	
	return a;
}

double VecB::pop_back()
{
	if (N_ == 0) throw std::runtime_error("VecB: nothing to pop");
	double a = x_[N_-1];

	double * temp = new double[N_ - 1];
	for(long i = 0; i != N_ - 1; ++i) temp[i] = x_[i];

	std::swap(temp, x_);
	delete [] temp;
	--N_;
	--ub_; // decreases ub by one

	return a;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   resize methods
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void VecB::resize(long ub)
{
	resize(1, ub);
}
	                                  
void VecB::resize(long lb, long ub)
{
	if (lb > ub) throw std::runtime_error("VecB: index bounds error");

	long N = 1 + ub - lb;
	double * temp = new double[N];
	for(long i = 0; i != N; ++i) temp[i] = 0.0;
	std::swap(temp, x_);
	delete [] temp;
	
	N_ = N;
	ub_ = ub;
	lb_ = lb;
}

void VecB::resize_preserve(long lb, long ub)
{
	if (lb > ub) throw std::runtime_error("VecB: index bounds error");

	long N = 1 + ub - lb;
	double * temp = new double[N];
	
	//XXXXXX if the ranges do not overlap XXXXXXXXXXXXXXX
	if (lb > ub_ || ub < lb_)
    {    for(long i = 0; i != N; ++i) temp[i] = 0.0;
    }
    else
    {
	    //XXXXXX lb < lb_ XXXXXXXXXXXXXXX
		if (lb < lb_)
		{
            for(long i = 0; i != lb_ - lb; ++i) temp[i] = 0.0;
        	if (ub <= ub_)
                for(long i = 0; i <= ub - lb_; ++i) temp[lb_ - lb + i] = x_[i];
		    else
			{
		        for(long i = 0; i <= ub_ - lb_; ++i) temp[lb_ - lb + i] = x_[i];
			    for(long i = 1 + ub_ - lb; i != N; ++i) temp[i] = 0.0;
		    }
        }
        else
	    //XXXXXX lb_ <= lb <= ub_ XXXXXXXXX
        {
            if (ub <= ub_)
                for(long i = 0; i != N; ++i) temp[i] = x_[lb - lb_ + i];
		    else
		    {
			    for(long i = 0; i <= ub_ - lb; ++i) temp[i] = x_[lb - lb_ + i];
			    for(long i = 1 + ub_ - lb; i != N; ++i) temp[i] = 0.0;
		    }
	    }
	}
	
	std::swap(temp, x_);
	delete [] temp;

	N_ = N;
	ub_ = ub;
	lb_ = lb;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   private method
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void VecB::CheckIndex(long i) const
{
	if ((i >= lb_)&&(i <= ub_)) return;

	std::ostringstream o;
	o << lb_ << " " << i << " " << ub_;
	throw std::runtime_error("VecB: index range error:  " + o.str());
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

