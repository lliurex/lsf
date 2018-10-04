
#include <lsf-1.0/system.hpp>
#include <lsf-1.0/common.hpp>

#include <unistd.h>
#include <dirent.h>
#include <sys/types.h> 
#include <sys/stat.h> 

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>

using namespace std;
using namespace net::lliurex;



bool isNumeric(const char * name)
{
	int n = 0;
	bool is_num = true;
	
	while(name[n]!='\0')
	{
		if(name[n]<'0' || name[n]>'9')
		{
			is_num=false;
			break;
		}
		n++;
	}
	
	return is_num;
}

vector<unsigned int> net::lliurex::system::GetProcessList()
{
	vector<unsigned int> plist;
	
	DIR* dir_proc = NULL ;
	struct dirent* dir_entity;
	
	/* Yeah, I know, the path is hardcoded, feel free to change it, if  you wish*/
	dir_proc = opendir("/proc/") ;
	
	if (dir_proc == NULL)
	{
		throw Exception("Failed to access /proc/");
	}
	
		
	while((dir_entity = readdir(dir_proc)))
	{
		if (dir_entity->d_type == DT_DIR)
		{
			if(isNumeric(dir_entity->d_name))
			{
				plist.push_back(atoi(dir_entity->d_name));
			}
		}
		
	}
	
	return plist;
	    
}

system::ProcessInfo net::lliurex::system::GetProcessInfo(unsigned int pid)
{
	system::ProcessInfo pinfo;
	stringstream ss;
	ifstream * ifile;
	char data[256];
	
	ss<<"/proc/"<<pid<<"/cmdline";
	
	ifile = new ifstream();
	ifile->open(ss.str().c_str());
	ifile->getline(data,256);
	ifile->close();
	delete ifile;
	
	pinfo.cmdline=string(data);
	
	ss.str("");
	ss<<"/proc/"<<pid<<"/comm";
	
	ifile = new ifstream();
	ifile->open(ss.str().c_str());
	ifile->getline(data,256);
	ifile->close();
	delete ifile;
	
	pinfo.comm=string(data);
	
	return pinfo;
}

std::vector<std::string> net::lliurex::system::GetModuleList()
{
	std::vector<std::string> mlist;
	ifstream ifile;
	stringstream ss;
	string str;
	char data[256];
	
	
	
	ifile.open("/proc/modules");
	while(true)
	{
		ifile.getline(data,256);
		if(ifile.eof())break;
		
		ss.str(data);
		ss>>str;
		mlist.push_back(str);
		
	}
	ifile.close();
	
	return mlist;
}

std::vector<system::MountInfo> net::lliurex::system::GetMountList()
{
	std::vector<system::MountInfo> mlist;
	ifstream ifile;
	stringstream ss;
	char data[256];
	system::MountInfo minfo;
	string str;
	
		
	ifile.open("/proc/mounts");
	while(true)
	{
			
		ifile>>minfo.device;
		ifile>>minfo.mount_point;
		ifile>>minfo.type;
		ifile>>minfo.options;
		
		ifile>>str;
		minfo.dumb=(str=="0") ? false : true;
		
		ifile>>str;
		minfo.pass=(str=="0") ? false : true;
		
		if(ifile.eof())break;
		
		mlist.push_back(minfo);
	}
	
	ifile.close();
	
	return mlist;
}

long net::lliurex::system::GetSystemMemory()
{
	long pages = sysconf(_SC_PHYS_PAGES);
	long page_size = sysconf(_SC_PAGE_SIZE);
	return pages * page_size;
}

long net::lliurex::system::GetFreeSystemMemory()
{
	long pages = sysconf(_SC_AVPHYS_PAGES);
	long page_size = sysconf(_SC_PAGE_SIZE);
	return pages * page_size;
}
	
