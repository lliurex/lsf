

#ifndef _LSF_COMMON_
#define _LSF_COMMON_

#include <string>


namespace net
{
	namespace lliurex
	{
		class Exception
		{
			private:
				std::string s;
			
			public:
			
				Exception(std::string ss) : s(ss) {}
				~Exception() throw () {}
				const char* what() const throw() { return s.c_str(); }
		};
	}
}


#endif
