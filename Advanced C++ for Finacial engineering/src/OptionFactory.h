//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  OptionFactory.h
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#ifndef OptionFactoryH
#define OptionFactoryH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <string>
#include <map>

class MonitorManager;
class OptionBase;
class Parameters;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  class OptionFactory
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXvXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

class OptionFactory
{
	public:
		~OptionFactory(){}

        static OptionFactory & Instance();

		typedef OptionBase * (*DerivedClassCreator)(const Parameters &);

        typedef std::pair<std::string, DerivedClassCreator> CreatePair;
		typedef std::map<std::string, DerivedClassCreator> CreateMap;

        typedef std::pair<std::string, std::string> LookUpPair;
		typedef std::map<std::string, std::string> LookUpMap;

		void RegisterObject(const std::string &, const std::string &, DerivedClassCreator);

		OptionBase * Create(const std::string &, const Parameters &);

	private:
		OptionFactory();
		OptionFactory(const OptionFactory &);
		OptionFactory & operator=(const OptionFactory &);

        CreateMap CreateObjectsMap_;
        LookUpMap IDLookUpMap_;
        
        MonitorManager * mon_;
		
		void DoMonitorOutput(OptionBase &);
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

