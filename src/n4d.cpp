
#include <lsf-1.0/n4d.hpp>

#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

#include <iostream>
#include <vector>




using namespace std;
using namespace net::lliurex;


xmlrpc_c::value n4d::GetVariableList(string server_addr)
{
	xmlrpc_c::clientXmlTransport_curl *transport;
	xmlrpc_c::client_xml *client;
	
	
	transport=new xmlrpc_c::clientXmlTransport_curl(
                xmlrpc_c::clientXmlTransport_curl::constrOpt()
                .no_ssl_verifyhost(true)
                .no_ssl_verifypeer(true)
        );
	
        client=new xmlrpc_c::client_xml(transport);

	xmlrpc_c::paramList params;
	params.add(xmlrpc_c::value_string(""));
	params.add(xmlrpc_c::value_string("VariablesManager"));
	xmlrpc_c::rpcPtr caller("get_variables",params);
	xmlrpc_c::carriageParm_curl0 carriage(server_addr);

	caller->call(client,&carriage);
	
	return caller->getResult();
	
}


xmlrpc_c::value n4d::GetVariable(std::string varname, std::string server_addr)
{
	xmlrpc_c::clientXmlTransport_curl *transport;
	xmlrpc_c::client_xml *client;
	
	
	transport=new xmlrpc_c::clientXmlTransport_curl(
                xmlrpc_c::clientXmlTransport_curl::constrOpt()
                .no_ssl_verifyhost(true)
                .no_ssl_verifypeer(true)
        );
	
        client=new xmlrpc_c::client_xml(transport);

	xmlrpc_c::paramList params;
	params.add(xmlrpc_c::value_string(""));
	params.add(xmlrpc_c::value_string("VariablesManager"));
	params.add(xmlrpc_c::value_string(varname));
	xmlrpc_c::rpcPtr caller("get_variable",params);
	xmlrpc_c::carriageParm_curl0 carriage(server_addr);

	caller->call(client,&carriage);
	
	return caller->getResult();
}

void n4d::SetVariable(std::string varname,xmlrpc_c::value data,std::string user,std::string password,std::string server_addr)
{
	xmlrpc_c::clientXmlTransport_curl *transport;
	xmlrpc_c::client_xml *client;
	
	
	transport=new xmlrpc_c::clientXmlTransport_curl(
                xmlrpc_c::clientXmlTransport_curl::constrOpt()
                .no_ssl_verifyhost(true)
                .no_ssl_verifypeer(true)
        );
	
        client=new xmlrpc_c::client_xml(transport);

	vector<xmlrpc_c::value> login;
			
	login.push_back(xmlrpc_c::value_string(user));
	login.push_back(xmlrpc_c::value_string(password));
	
	xmlrpc_c::paramList params;
	params.add(xmlrpc_c::value_array(login));
	params.add(xmlrpc_c::value_string("VariablesManager"));
	params.add(xmlrpc_c::value_string(varname));
	params.add(data);
	xmlrpc_c::rpcPtr caller("set_variable",params);
	xmlrpc_c::carriageParm_curl0 carriage(server_addr);

	caller->call(client,&carriage);
	
	xmlrpc_c::value ret=caller->getResult();
	
	if(ret.type()==xmlrpc_c::value::TYPE_STRING)
	{
		xmlrpc_c::value_string sret(ret);
		cout<<((string)sret)<<endl;
	}
		
}