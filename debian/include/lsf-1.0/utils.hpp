
#ifndef _LSF_UTILS_
#define _LSF_UTILS_

#include <string>
#include <vector>

namespace net
{
	namespace lliurex
	{
		namespace utils
		{
			namespace str
			{
				/*!
					Splits a string, using split character
					\param s source string
					\param split character used for splitting
					\return vector containing splitted strings
				*/ 
				std::vector<std::string> Split(std::string s,char split);
				
				/*!
					Strips whitespaces and newlines characters from both sides of the string
					\param s source string
					\return the stripped string
				*/ 
				std::string Strip(std::string s);
				
				/*!
					Replace matched substrings
					\param s source string
					\param match substring to replace
					\param replace string used for replacement
					\return replaced string
				*/ 
				std::string Replace(std::string s, std::string match,std::string replace);
			}
			
			namespace parse
			{
				/*!
					Checks whenever a string matches the given regex pattern
					\param s source string
					\param pattern regex
				*/ 
				bool Match(std::string s,std::string pattern);
				
				/*!
					Checks whenever a string can be parsed as an integer
					\param s source string
				*/ 
				bool IsInteger(std::string s);
				
				/*!
					Checks whenever a string can be parsed as a float
					\param s source string
				*/ 
				bool IsFloat(std::string s);
			}
		}
	}
}

#endif
