
#ifndef _LSF_SYSTEM_
#define _LSF_SYSTEM_

#include <string>
#include <vector>

namespace net
{
	namespace lliurex
	{
		namespace system
		{
			class ProcessInfo
			{
				public:
					std::string cmdline; /*!< complete command line string */
					std::string comm; /*!< exec name */
			};
			
			class MountInfo
			{
				public:
					std::string device; /*!< device mount point */
					std::string mount_point; /*!< mount point path */
					std::string type; /*!< type of mount */
					std::string options; /*!< mount options */
					bool dumb; /*!< dumb mode */
					bool pass; /*!< pass mode */
			};
			
			/*!
				fills the vector with the current list of process id
				\returns Output vector where list is stored
			*/ 
			std::vector<unsigned int> GetProcessList();
			
			/*!
				Gets basic process info, with a given PID
				\param pid Process id
				\return A ProcessInfo class
			*/
			ProcessInfo GetProcessInfo(unsigned int pid);
			
			/*!
				Gets the list of current loaded modules
				\returns Output vector where list is stored
			*/
			std::vector<std::string> GetModuleList();
			
			/*!
				Gets current mount points
				\returns Output vector where list is stored
			*/
			std::vector<MountInfo> GetMountList();
			
			/*!
				Gets total amount of system memory (RAM)
			*/
			long GetSystemMemory();
			
			/*!
				Gets amount of available system memory
			*/
			long GetFreeSystemMemory();
		}
	}
}
#endif
