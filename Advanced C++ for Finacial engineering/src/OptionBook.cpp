//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     OptionBook
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <stdexcept>
#include <vector>

#include "VecB.h"

#include "OptionBook.h"

#include "OptionBase.h"
#include "OptionIOManager.h"
#include "OptionFactory.h"
#include "Parameters.h"

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     structural
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

OptionBook::OptionBook()
{
    OptionIOManager & io = OptionIOManager::Instance();
    
    Nopts_ = io.GetNOptions();

    opt_.resize(Nopts_);                //options

    for (long i = 0; i != Nopts_; ++i)
	{
		std::string otype = io.GetType(i);
		Parameters oparas = io.GetParas(i);
        opt_[i] = OptionFactory::Instance().Create(otype, oparas);
	}
}

OptionBook::~OptionBook()
{
    for(long i = 0; i != Nopts_; ++i)
    {
		delete opt_[i];
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     interface
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void OptionBook::initialize(double Tmax, long N)
{
    for(long i = 0; i != Nopts_; ++i)
    {
        opt_[i]->initialize(Tmax, N);
	}
}

void OptionBook::ReceivePath(const std::vector<double> & path, VecB & results)
{
	results.resize(1, Nopts_);
    for(long i = 0; i != Nopts_; ++i)
    {
        results[i+1] = opt_[i]->ComputePayoffPath(path);	//compute each option payoff
	}
}

OptionBase * OptionBook::GetOption(long i) const
{
	if (i < 1 || i > Nopts_) throw std::runtime_error("OptionBook::GetOption.  Index out of range");
	return opt_[i-1];
}

double OptionBook::GetT(long i)
{
	return opt_[i-1]->GetT();
}

long OptionBook::GetOptionID(long i)
{
	return opt_[i-1]->GetOptionID();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XX     end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

