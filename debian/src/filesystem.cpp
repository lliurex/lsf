
#include <lsf-1.0/filesystem.hpp>

#include <sys/inotify.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <glob.h>
#include <libgen.h>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;
using namespace net::lliurex;

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

bool filesystem::Exists(string path)
{
	return (access( path.c_str(), F_OK )!=-1);
}

bool filesystem::IsDir(string path)
{
	struct stat fflags;
	
	stat(path.c_str(),&fflags);
	
	return S_ISDIR(fflags.st_mode);
}

std::vector<std::string> filesystem::ListDir(string path)
{
	struct stat fflags;
	std::vector<std::string> files;
	
	stat(path.c_str(),&fflags);
	
	if(S_ISDIR(fflags.st_mode))
	{

		
		DIR *dp;
		struct dirent *dirp;
		
		if((dp  = opendir(path.c_str())) == NULL)
		{
			cout << "Error opening " << path << endl;
		}

		while ((dirp = readdir(dp)) != NULL)
		{
			files.push_back(string(dirp->d_name));
		}
		
		closedir(dp);
		
	}
	
	return files;
}

std::vector<std::string> filesystem::List(string expression)
{
	glob_t glob_result;
	glob(expression.c_str(),GLOB_TILDE,NULL,&glob_result);
	
	std::vector<std::string> files;
	
	for(int n=0;n<glob_result.gl_pathc;n++)
	{
		files.push_back(string(glob_result.gl_pathv[n]));
	}
    
	globfree(&glob_result);
	
	return files;
}

string filesystem::Home()
{
	struct passwd *pw = getpwuid(getuid());
	return string(pw->pw_dir);
}

void CreateDirAux(string path)
{
	
	if(!filesystem::Exists(path))
	{
		int pos = path.rfind("/");
		if(pos!=string::npos)
		{
			CreateDirAux(path.substr(0,pos));
			mkdir(path.c_str(),S_IRWXU);
		}
	}		
	
	
}

void filesystem::CreateDir(string path,bool create_path)
{
	if(create_path)
	{
		if(path[path.length()-1]=='/')
			CreateDirAux(path.substr(0,path.length()-1));
		else
			CreateDirAux(path);
	}
	else
	{
		mkdir(path.c_str(),S_IRWXU);
	}
}

string filesystem::GetWorkingDir()
{
	int len=128;
	char * buff;
	
	repeat:
	
	buff = new char[len];
	
	if(getcwd(buff,len)==NULL)
	{
		delete buff;
		len*=2;
		goto repeat;
	}
	
	string ret(buff);
	delete buff;
	
	return ret;
}


string filesystem::BaseName(string path)
{
	char * dup = new char[path.length()+1];
	strcpy(dup,path.c_str());
	string ret(basename(dup));
	
	delete dup;
	
	return ret;
}
		
string filesystem::DirName(string path)
{
	char * dup = new char[path.length()+1];
	strcpy(dup,path.c_str());
	string ret(dirname(dup));
	
	delete dup;
	
	return ret;
}

filesystem::NotifyHandler::NotifyHandler(string path)
{
	this->path=path;
	
	inotify_fd = inotify_init();

	wd = inotify_add_watch(inotify_fd, path.c_str(), IN_ALL_EVENTS);
	
	if(wd==-1)
	{
		cerr<<"Error adding inotify watch"<<endl;
	}
	
	
}

filesystem::NotifyHandler::~NotifyHandler()
{
	cout<<"NotifyHandler: removing watch"<<endl;
	inotify_rm_watch( inotify_fd, wd );
	close( inotify_fd );
}

void filesystem::NotifyHandler::Push()
{
	
	ssize_t num_read;
	char buf[BUF_LEN];
	struct inotify_event *event;
	
	num_read = read(inotify_fd, buf, BUF_LEN);
	
	if(num_read>0)
	{
		char * p = buf;
		while(p < buf + num_read)
		{
			event = (struct inotify_event *) p;
			
			if(event->mask & IN_ACCESS)OnAccess(string(event->name));
			if(event->mask & IN_MODIFY)OnModify(string(event->name));
			if(event->mask & IN_ATTRIB)OnAttrib(string(event->name));
			if(event->mask & IN_CLOSE_WRITE)OnCloseWrite(string(event->name));
			if(event->mask & IN_CLOSE_NOWRITE)OnCloseNoWrite(string(event->name));
			if(event->mask & IN_OPEN)OnOpen(string(event->name));
			if(event->mask & IN_MOVED_FROM)OnMovedFrom(string(event->name));
			if(event->mask & IN_MOVED_TO)OnMovedTo(string(event->name));
			if(event->mask & IN_CREATE)OnCreate(string(event->name));
			if(event->mask & IN_DELETE)OnDelete(string(event->name));
			if(event->mask & IN_DELETE_SELF)OnDeleteSelf(string(event->name));
			
			p=p+sizeof(struct inotify_event) + event->len;
		}
	}
	else
	{
		OnError();
	}
}

void filesystem::NotifyHandler::OnError()
{
	cerr<<"NotifyHandler: Error reading from inotify"<<endl;
}
	

/* File accessed */
void filesystem::NotifyHandler::OnAccess(std::string path)
{
	cout<<"NotifyHandler::OnAcess("<<path<<")"<<endl;
}

/* File modified */
void filesystem::NotifyHandler::OnModify(std::string path)
{
	cout<<"NotifyHandler::OnModify("<<path<<")"<<endl;
}

/* metadata changed */
void filesystem::NotifyHandler::OnAttrib(std::string path)
{
	cout<<"NotifyHandler::OnAttrib("<<path<<")"<<endl;
}

/* Writable file closed */
void filesystem::NotifyHandler::OnCloseWrite(std::string path)
{
	cout<<"NotifyHandler::OnCloseWrite("<<path<<")"<<endl;
}

/* Unwritable file closed */
void filesystem::NotifyHandler::OnCloseNoWrite(std::string path)
{
	cout<<"NotifyHandler::OnCloseNoWrite("<<path<<")"<<endl;
}

/* File opened */
void filesystem::NotifyHandler::OnOpen(std::string path)
{
	cout<<"NotifyHandler::OnOpen("<<path<<")"<<endl;
}

/* File moved from */
void filesystem::NotifyHandler::OnMovedFrom(std::string path)
{
	cout<<"NotifyHandler::OnMovedFrom("<<path<<")"<<endl;
}

/* File moved to */
void filesystem::NotifyHandler::OnMovedTo(std::string path)
{
	cout<<"NotifyHandler::OnMovedTo("<<path<<")"<<endl;
}

/* File created */
void filesystem::NotifyHandler::OnCreate(std::string path)
{
	cout<<"NotifyHandler::OnCreate("<<path<<")"<<endl;
}

/* File deleted */
void filesystem::NotifyHandler::OnDelete(std::string path)
{
	cout<<"NotifyHandler::OnDelete("<<path<<")"<<endl;
}

/* notify path deleted */
void filesystem::NotifyHandler::OnDeleteSelf(std::string path)
{
	cout<<"NotifyHandler::OnDeleteSelf("<<path<<")"<<endl;
}
