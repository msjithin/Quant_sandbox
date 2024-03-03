//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionIOManager.cpp.
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>
#include <iostream>

#include "OptionIOManager.h"

#include "OptionOutputManager.h"
#include "OptionInputManager.h"
#include "Parameters.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionIOManager::OptionIOManager()
:   out_(0)  
,   inp_(0)
{
    out_ = new OptionOutputManager; //owns
    inp_ = new OptionInputManager;  //owns
}

OptionIOManager::~OptionIOManager()
{
	delete out_;
	delete inp_;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	getters
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionOutputManager * OptionIOManager::GetOptionOutputManager()
{return out_;}

long OptionIOManager::GetNOptions() const {return inp_->GetNOptions();}

std::string OptionIOManager::GetType(const long i)
{return inp_->GetType(i);}

Parameters OptionIOManager::GetParas(const long i)
{return inp_->GetParas(i);}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
