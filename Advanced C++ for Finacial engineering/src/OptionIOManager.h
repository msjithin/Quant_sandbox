//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionIOManager.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef OptionIOManagerH
#define OptionIOManagerH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>
#include <map>

class OutputManagerBase;
class OptionOutputManager;
class OptionInputManager;
class Parameters;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	OptionIOManager
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class OptionIOManager
{
    public:
		typedef std::map<std::string, double> ParamMap;
        typedef std::pair<std::string, ParamMap> OptionSpec;

		~OptionIOManager();

        static OptionIOManager & Instance()
		{
			static OptionIOManager this_manager;
			return this_manager;
		}

        OptionOutputManager * GetOptionOutputManager();
 
		long GetNOptions() const;
		std::string GetType(const long i);
		Parameters GetParas(const long i);

	private:
		OptionIOManager();
		OptionIOManager(const OptionIOManager &);
		OptionIOManager & operator=(const OptionIOManager &);

        OptionOutputManager * out_;   // owns
		OptionInputManager * inp_;    // owns
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//	end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX





