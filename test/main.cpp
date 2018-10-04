
#include <lsf-1.0/utils.hpp>
#include <lsf-1.0/system.hpp>
#include <lsf-1.0/network.hpp>
#include <lsf-1.0/n4d.hpp>
#include <lsf-1.0/filesystem.hpp>

#include <xmlrpc-c/base.hpp>
#include <signal.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>

using namespace net::lliurex;
using namespace std;

bool quit_request=false;

void signal_handler(int sig)
{
	quit_request=true;
}

int main(int argc,char * argv[])
{
		 
	
	
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = signal_handler;
	sigaction(SIGTERM, &action, NULL);
	
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = signal_handler;
	sigaction(SIGINT, &action, NULL);
	
	vector<string> options;
	
	for(int n=1;n<argc;n++)
	{
		options.push_back(string(argv[n]));
	}
	
	for(int n=0;n<options.size();n++)
	{
		if(options[n]=="utils")
		{
			cout<<"*** testing utils ***"<<endl;
			
			cout<<"*******************"<<endl;
			cout<<"utils::str::Split:"<<endl;
			cout<<"*******************"<<endl;
			
			vector<string> stack = utils::str::Split("  hello    world   ",' ');
			
			for(int n=0;n<stack.size();n++)
			{
				cout<<"["<<stack[n]<<"]"<<endl;
			}
			
			cout<<"*******************"<<endl;
			cout<<"utils::str::Strip:"<<endl;
			cout<<"*******************"<<endl;
			
			string stripped = utils::str::Strip("  oh\nyeah  \n");
			cout<<"["<<stripped<<"]"<<endl;
			
			cout<<"*******************"<<endl;
			cout<<"utils::str::Replace:"<<endl;
			cout<<"*******************"<<endl;
			
			string replaced = utils::str::Replace("It is time to {0} and chew {1}","{0}","kick butts");
			replaced = utils::str::Replace(replaced,"{1}","bubble gum");
			replaced = utils::str::Replace(replaced,"butts","ass");
			cout<<"["<<replaced<<"]"<<endl;
			
			cout<<"*******************"<<endl;
			cout<<"utils::parse::IsInteger:"<<endl;
			cout<<"*******************"<<endl;
			
			string num="234";
			bool isnum = utils::parse::IsInteger(num);
			cout<<"- status: "<<num<<"->"<<isnum<<endl;
			
			num="f234";
			isnum = utils::parse::IsInteger(num);
			cout<<"- status: "<<num<<"->"<<isnum<<endl;
			
			
			cout<<"*******************"<<endl;
			cout<<"utils::parse::IsFloat:"<<endl;
			cout<<"*******************"<<endl;
			
			num="-3.14";
			isnum = utils::parse::IsFloat(num);
			cout<<"- status: "<<num<<"->"<<isnum<<endl;
			
			num="45.e";
			isnum = utils::parse::IsFloat(num);
			cout<<"- status: "<<num<<"->"<<isnum<<endl;
			
			
		}
		
		if(options[n]=="system")
		{
			cout<<"*** testing system ***"<<endl;
					
			
			cout<<"*******************"<<endl;
			cout<<"system::GetProcessList:"<<endl;
			cout<<"*******************"<<endl;
			
			vector<unsigned int > plist;
			
			plist=system::GetProcessList();
				
			for(int n=0;n<plist.size();n++)
			{
					system::ProcessInfo pinfo = system::GetProcessInfo(plist[n]);
					if(pinfo.cmdline=="")
						cout<<"* "<<plist[n]<<": ["<<pinfo.comm<<"]"<<endl;
					else
						cout<<"* "<<plist[n]<<": "<<pinfo.cmdline<<endl;
			}
			
			
			cout<<"*******************"<<endl;
			cout<<"system::GetModuleList:"<<endl;
			cout<<"*******************"<<endl;
			vector<string> mlist;
			
			mlist=system::GetModuleList();
			
			for(int n=0;n<mlist.size();n++)
			{
				cout<<"- "<<mlist[n]<<endl;
			}
			
			cout<<"*******************"<<endl;
			cout<<"system::GetMountList:"<<endl;
			cout<<"*******************"<<endl;
			vector<system::MountInfo> minfo;
			
			minfo=system::GetMountList();
			
			for(int n=0;n<minfo.size();n++)
			{
				cout<<"* device: "<<minfo[n].device<<" on "<<minfo[n].mount_point<<" type: "<<minfo[n].type<<endl;
			}
			
			cout<<"*******************"<<endl;
			cout<<"system::GetSystemMemory:"<<endl;
			cout<<"system::GetFreeSystemMemory:"<<endl;
			cout<<"*******************"<<endl;
			cout<<"System memory: "<<system::GetSystemMemory()/1024<<"KB"<<endl;
			cout<<"Free system memory: "<<system::GetFreeSystemMemory()/1024<<"KB"<<endl;
			
		}// system
		
		if(options[n]=="network")
		{
			cout<<"*******************"<<endl;
			cout<<"network::GetDeviceList:"<<endl;
			cout<<"*******************"<<endl;
			
			vector<network::DeviceInfo> dlist;
			
			dlist=network::GetDeviceList();
			
			for(int n=0;n<dlist.size();n++)
			{
				cout<<"* interface: "<<dlist[n].name<<endl;
				cout<<"\t-MAC:"<<network::MACToString(dlist[n].mac)<<endl;
				cout<<"\t-address:"<<network::AddressToString(dlist[n].address)<<endl;
				cout<<"\t-link:"<<dlist[n].link<<endl;
				cout<<"\t-promisc:"<<dlist[n].promisc<<endl;
				
			}
		}//network
			
		if(options[n]=="n4d")
		{
			cout<<"*******************"<<endl;
			cout<<"n4d::GetVariableList:"<<endl;
			cout<<"*******************"<<endl;

			xmlrpc_c::value_struct ret_struct(n4d::GetVariableList("https://anubis:9779"));
		
			map<string,xmlrpc_c::value>ret = ret_struct;
			map<string,xmlrpc_c::value>::iterator iter;
			
			for(iter=ret.begin();iter!=ret.end();iter++)
			{
				xmlrpc_c::value_struct val(iter->second);
				map<string,xmlrpc_c::value>val_map=val;
				
				if(val_map["value"].type()==xmlrpc_c::value::TYPE_STRING)
				{
					xmlrpc_c::value_string strv(val_map["value"]);
					cout<<"* item: "<<iter->first<<" --> "<<((string)strv)<<endl;
				}
				else cout<<"* item: "<<iter->first<<" type of "<<val_map["value"].type()<<endl;
					
			}
			
			cout<<"*******************"<<endl;
			cout<<"n4d::SetVariable:"<<endl;
			cout<<"*******************"<<endl;
			xmlrpc_c::value_string var_set("lliurex was here");
			n4d::SetVariable("VARIABLE2",var_set,"cless","lliurex","https://anubis:9779");
			
			cout<<"*******************"<<endl;
			cout<<"n4d::GetVariable:"<<endl;
			cout<<"*******************"<<endl;
			
			xmlrpc_c::value var_ret=n4d::GetVariable("VARIABLE2","https://anubis:9779");
			
			if(var_ret.type()==xmlrpc_c::value::TYPE_STRING)
			{
				xmlrpc_c::value_string vstr(var_ret);
				cout<<"* value: "<<((string)vstr)<<endl;
			}
			
			
			
			
			
		}//n4d
		
		if(options[n]=="filesystem")
		{
			
			cout<<"*******************"<<endl;
			cout<<"filesystem::GetWorkingDir:"<<endl;
			cout<<"filesystem::Exists:"<<endl;
			cout<<"filesystem::IsDir:"<<endl;
			cout<<"filesystem::ListDir:"<<endl;
			cout<<"filesystem::List:"<<endl;
			cout<<"filesystem::DirName:"<<endl;
			cout<<"filesystem::BaseName:"<<endl;
			cout<<"filesystem::Home:"<<endl;
			cout<<"filesystem::CreateDir:"<<endl;
			cout<<"*******************"<<endl;
			
			cout<<"Working dir: "<<filesystem::GetWorkingDir()<<endl;
			
			if(filesystem::Exists("/tmp"))
			{
				vector<string> files;
				files=filesystem::ListDir("/tmp");
				
				for(int n=0;n<files.size();n++)
				{
					if(filesystem::IsDir("/tmp/"+files[n]))
						cout<<"\tDir: "<<files[n]<<endl;
					else
						cout<<"\tFile: "<<files[n]<<endl;
				}
				
				cout<<"Home: "<<filesystem::Home()<<endl;
				filesystem::CreateDir("/tmp/alpha/bravo/charlie",true);
				
				
			}
			else
			{
				cout<<"/tmp does not exists in the filesystem"<<endl;
			}
			
			vector<string> match;
			match=filesystem::List("/*/*.conf");
			string dirname="";
			for(int n=0;n<match.size();n++)
			{
				string dname = filesystem::DirName(match[n]);
				if(dname!=dirname)
				{
					dirname=dname;
					cout<<dirname<<":"<<endl;
				}
				cout<<"\t"<<filesystem::BaseName(match[n])<<endl;
				
				
			}
			
		}//filesystem
		
		if(options[n]=="filesystem-notify")
		{
			cout<<"*******************"<<endl;
			cout<<"filesystem::NotifyHandler:"<<endl;
			cout<<"*******************"<<endl;
			filesystem::NotifyHandler nh("/tmp"); 
			
			while(!quit_request)
			{
				nh.Push();
			}
		}//filesystem-notify
	}
	
	
	
}
