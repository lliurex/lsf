
#ifndef _LSF_N4D_
#define _LSF_N4D_

#include <xmlrpc-c/base.hpp>

#include <vector>
#include <string>

namespace net
{
	namespace lliurex
	{
		namespace n4d
		{
			/*!
				Gets available variables
				\param server_addr address of a valid n4d server
			*/
			xmlrpc_c::value GetVariableList(std::string server_addr=std::string("https://localhost:9779"));
			/*!
				Reads a variable
				\param varname variable name
				\param server_addr address of a valid n4d server 
			*/
			xmlrpc_c::value GetVariable(std::string varname, std::string server_addr=std::string("https://localhost:9779"));
			
			/*!
				Sets a variable
				\param varname variable name
				\param data variable value
				\param user user name
				\param password user password
				\param server_addr address of a valid n4d server
			*/
			void SetVariable(std::string varname,xmlrpc_c::value data,std::string user,std::string password,std::string server_addr=std::string("https://localhost:9779"));
			
		}
	}
}

#endif
