
#ifndef _LSF_NETWORK_
#define _LSF_NETWORK_


#include <vector>
#include <string>


namespace net
{
	namespace lliurex
	{
		namespace network
		{
			class DeviceInfo
			{
				public:
					std::string name; /*!< device name */
					unsigned char mac[6]; /*!< MAC address */ 
					unsigned long address; /*!< ip address */
					bool link; /*!< link status */
					bool promisc; /*!< promiscuous mode */
			};
			
			/*!
				Gets a vector with current network devices settings
				\return the vector 
			*/
			std::vector<DeviceInfo> GetDeviceList();
			
			/*!
				Converts ip address to string
			*/
			std::string AddressToString(unsigned long address);
			
			/*!
				Converts MAC address to string
			*/
			std::string MACToString(unsigned char * mac);
		}
	}
}

#endif
