//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	WienerStratify.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <cmath>
#include <string>

#include "rv_library.h"
#include "lib_val.h"
#include "VecB.h"

#include "WienerStratify.h"

#include "IO_Manager.h"
#include "ParameterN.h"
#include "ParameterM.h"
#include "ParameterTmax.h"

#include "AppFactoryRegistration.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  traits
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

template<> std::string ClassName<WienerStratify>()
{
    static std::string name = "WienerStratify";
    return name;
}

template<> std::string ClassID<WienerStratify>()
{
    static std::string name = "s";  // stratified (at final time)
    return name;
}

template<> std::string BaseClassName<WienerStratify>()
{
    return ClassName<WienerBase>();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  register with factory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

namespace
{
	AppFactoryRegistration<WienerBase, WienerStratify> RegisterWienerStratify;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	Constructor
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

WienerStratify::WienerStratify()
:   N_(IO_Manager::Instance().GetValue<long>(ParameterN()))
,   M_(IO_Manager::Instance().GetValue<long>(ParameterM()))
,   j_(0)
,   z0_(0.0)
{
	double Tmax = IO_Manager::Instance().GetValue<double>(ParameterTmax());
    dt_ =  Tmax / N_;  						  // time step

    rt_ = std::sqrt(dt_);            		  // speed up
}

WienerStratify::~WienerStratify()
{}
 						 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FillUpPath()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void WienerStratify::FillUpPath(VecB & path)
{
	if (j_ == M_) j_ = 0;

    long lb = path.LBound();
    long ub = path.UBound();

    path[lb] = z0_;
    
	double u = rv::my_ran2();
    double v = (j_ + u)/M_;     //stratified
    path[ub] = rv::cndev(v);
    ++j_;
    
	long i = long(std::floor(0.5*(1 + lb + ub)));

    Get_Brownian_bridge(path, dt_, lb, i, ub);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	FillUpSlice()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void WienerStratify::FillUpSlice(const double T, VecB & slice) const
{
    long lb = slice.LBound();
    long ub = slice.UBound();
    
    double rT = std::sqrt(N_ * dt_);
    
    for(long i = lb;  i <= ub; ++i)     // uncorrelated random sample for time T
    {
	    double w = rv::GetNormalVariate();
       	slice[i] = w * rT;;
    }
}
                 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	next_z()
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double WienerStratify::next_z(double z)
{
    double w = rv::GetNormalVariate();
    return z + rt_*w;  // adds a Wiener increment
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	CreatableBase stuff
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

std::string WienerStratify::GetName() const {return ClassName<WienerStratify>();}
std::string WienerStratify::GetID() const {return ClassID<WienerStratify>();}
std::string WienerStratify::GetBaseClassName() const {return BaseClassName<WienerStratify>();}
std::string WienerStratify::GetStringify() const
{
    std::string nm = GetBaseClassName() + " is " + GetName() + '\n';
    nm += "  z0 =  " + lv::DoubleToString(z0_) + '\n';
    nm += "  N =   " + lv::DoubleToString(N_) + '\n';
    nm += "  T =   " + lv::DoubleToString(N_*dt_) + '\n';
    return nm;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX	Get_Brownian_bridge.  uses VecB
//XX    Constructs a Brownian bridge between i and N, in binary chops,
//XX    Uses recursion to get lefthand and righthand sides.
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void WienerStratify::Get_Brownian_bridge(VecB & Wpath, double dt, long i, long j, long N)
{
    if (i == j || j == N) return;

    double c_1 = (N - j) / double(N - i);
	double c_2 = (j - i) / double(N - i);
	double c_3 = std::sqrt(dt * (N - j) * (j - i) / double(N - i));

	double z = rv::cndev(rv::my_ran2());

	Wpath[j] = c_1 * Wpath[i] + c_2 * Wpath[N] + c_3 * z;

	if (j > i + 1) Get_Brownian_bridge(Wpath, dt, i, long(std::floor(0.5 + 0.5*(i + j))), j);
	if (j < N - 1) Get_Brownian_bridge(Wpath, dt, j, long(std::floor(0.5 + 0.5*(j + N))), N);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX







