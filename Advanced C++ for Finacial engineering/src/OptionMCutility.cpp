//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Optionmcutility.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>
#include <iostream>
#include <stdexcept>
#include <iomanip>

#include "utility.h"
#include "rv_library.h"
#include "helper.h"
#include "OptionMCutility.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  interface 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

double OptionMCutility::next_S(const double &path_S, const double &drift, const double &sgrt)
{

    double w = rv::GetNormalVariate();

    return path_S*std::exp(drift + sgrt*w);

}



double OptionMCutility::ComputePayoff(const double &path_S, const double &X)
{

    return std::max(0.0, path_S - X);

}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  End of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX