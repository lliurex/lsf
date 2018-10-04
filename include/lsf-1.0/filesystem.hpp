
#ifndef _LSF_FILESYSTEM_
#define _LSF_FILESYSTEM_

#include <sys/inotify.h>

#include <string>
#include <vector>

namespace net
{
	namespace lliurex
	{
		namespace filesystem
		{
			/*!
				Check if path exists
			*/
			bool Exists(std::string path);
			
			/*!
				Check if given path is a directory
			*/
			bool IsDir(std::string path);
			
			/*!
				Returns a vector with the list of files and directories of the given path
			*/
			std::vector<std::string> ListDir(std::string path);
			
			/*!
				Returns a vector with the list of files and directories  matching the glob expression
			*/
			std::vector<std::string> List(std::string expression);
			
			/*!
				Returns current user home path
			*/
			std::string Home();
			
			/*!
				Creates a directory
				\param create_path If true, all directories in the path are created (the way mkdir -p does it)
			*/
			void CreateDir(std::string path,bool create_path=false);
			
			/*!
				Gets current working directory
			*/
			std::string GetWorkingDir();
			
			/*!
				Basename posix wrap
			*/
			std::string BaseName(std::string path);
			
			/*!
				Dirname posix wrap
			*/
			std::string DirName(std::string path);
			
			class NotifyHandler
			{
				protected:
					std::string path;
					int inotify_fd;
					int wd;
				
				public:
					
					/*!
						Constructor
						\param path The path to watch
					*/
					NotifyHandler(std::string path);
					~NotifyHandler();
				
					/*!
						Periodic calls to this method are needed if you want to update notifications
					*/
					void Push();
					
					/*!
						An error happened
					*/
					virtual void OnError();
				
					/*! 
						File accessed 
					*/
					virtual void OnAccess(std::string path);
					
					/*! 
						File modified 
					*/
					virtual void OnModify(std::string path);
					
					/*!
						metadata changed
					*/
					virtual void OnAttrib(std::string path);
					
					/*!
						Writable file closed
					*/
					virtual void OnCloseWrite(std::string path);
					
					/*!
						Unwritable file closed
					*/
					virtual void OnCloseNoWrite(std::string path);
					
					/*!
						File opened
					*/
					virtual void OnOpen(std::string path);
					
					/*!
						File moved from
					*/
					virtual void OnMovedFrom(std::string path);
					
					/*!
						File moved to
					*/
					virtual void OnMovedTo(std::string path);
					
					/*!
						File created
					*/
					virtual void OnCreate(std::string path);
					
					/*!
						File deleted
					*/
					virtual void OnDelete(std::string path);
					
					/*!
						notify path deleted
					*/
					virtual void OnDeleteSelf(std::string path);
				
					
			};
		}
	}
}
#endif
