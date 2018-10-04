
#include <lsf-1.0/utils.hpp>
#include <string>
#include <vector>

#include <regex.h>


using namespace net::lliurex::utils;
using namespace std;


vector<string> str::Split(string s,char split)
{
	vector<string>stack;
	string tmp;
	bool start=false;
		
	for(int n=0;n<s.length();n++)
	{
		if(s[n]==split)
		{
			if(start==true)
			{
				stack.push_back(tmp);
				tmp="";
				start=false;
			}
		}
		else
		{
			start=true;
			tmp=tmp+s[n];
		}
	}
	
	return stack;
}

string str::Strip(string s)
{
	
	
	int start=0;
	int end=s.length();
	
	for(int n=0;n<s.length();n++)
	{
		if(s[n]!=' ' && s[n]!='\n' && s[n]!='\t')
		{
			start=n;
			break;
		}
	}
	
	for(int n=s.length()-1;n>=0;n--)
	{
		if(s[n]!=' ' && s[n]!='\n' && s[n]!='\t')
		{
			end=n;
			break;
		}
	}
	
	return s.substr(start,end-1);
}

string str::Replace(string s,string match,string replace)
{
	int m=0;
		
	vector<int> pairs;
	
	for(int n=0;n<s.length();n++)
	{
		if(s[n]==match[m])m++;
			else m=0;
			
		if(m==match.length())
			{
				int p;
				p=n-m;
				p=p | (m<<16);
				pairs.push_back(p);
			}
	}
	
	for(int n=0;n<pairs.size();n++)
	{
		int pos = 0x0000FFFF & pairs[n];
		int len = (0xFFFF0000 & pairs[n])>>16;
		s.replace(pos+1,len,replace);
	}
	
	return s;
}

bool parse::Match(string s,string pattern)
{
	int    status;
    regex_t    re;

    if (regcomp(&re, pattern.c_str(), REG_EXTENDED|REG_NOSUB) != 0)
    {
		/* throw exception, maybe? */
		return false; 
    }
    
    status = regexec(&re, s.c_str(), (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return false;      
    }
    return true;
}

bool parse::IsInteger(string s)
{
	return parse::Match(s,"^[-+]?[0-9]+$");
}

bool parse::IsFloat(string s)
{
	/* there is room for improvement */
	return parse::Match(s,"^[-+]?[0-9]+\\.[0-9]+$");
}
