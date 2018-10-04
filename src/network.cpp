
#include <lsf-1.0/network.hpp>
#include <lsf-1.0/common.hpp>

#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>

#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


using namespace std;
using namespace net::lliurex;

/*
* Based on the example code of:
* Adam Pierce <adam@doctort.org>
*/
vector<network::DeviceInfo> network::GetDeviceList()
{
	
	int sck;
	struct ifconf ifc;
	struct ifreq *ifr;
	int nInterfaces;
	char buf[1024];

	vector<network::DeviceInfo> nlist;
	
	sck = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(sck < 0)
	{
		throw Exception("Failed to open the socket");
	}

	
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	
	if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
	{
		throw Exception("Failed on ioctl SIOCGIFCONF");
	}
	
	

	ifr = ifc.ifc_req;
	nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
	for(int i = 0; i < nInterfaces; i++)
	{
		struct ifreq *item = &ifr[i];
		struct ifreq  item_flags;
		struct ifreq item_mac;
		
		network::DeviceInfo dev;
		
		dev.name = string(item->ifr_name);
		dev.address=((struct sockaddr_in *)&item->ifr_addr)->sin_addr.s_addr;
		
		strncpy(item_flags.ifr_name,item->ifr_name,sizeof(item->ifr_name));
		int rv = ioctl(sck, SIOCGIFFLAGS, &item_flags);
		
		if(rv<0)
		{
			throw Exception("Failed on ioctl SIOCGIFFLAGS");
		}
		
		strcpy(item_mac.ifr_name,item->ifr_name);
		rv = ioctl(sck, SIOCGIFHWADDR, &item_mac);
		
		if(rv<0)
		{
			throw Exception("Failed on ioctl SIOCGIFHWADDR");
		}
		
		for(int n=0;n<6;n++)
		{
			dev.mac[n]=item_mac.ifr_addr.sa_data[n];
		}
		
		dev.link=(item_flags.ifr_flags & IFF_UP) && (item_flags.ifr_flags & IFF_RUNNING);
		dev.promisc=item_flags.ifr_flags & IFF_PROMISC;
		
		nlist.push_back(dev);
	}
	
	

	close(sck);
	
	return nlist;
}

/**
* Converts IPv4 address to string
*/
string network::AddressToString(unsigned long address)
{
	stringstream ss;
	union
	{
		unsigned long addr;
		unsigned char data[4];
	}span_addr;
	
	span_addr.addr=address;
	
	ss<<(int)span_addr.data[0]<<"."<<(int)span_addr.data[1]<<"."<<(int)span_addr.data[2]<<"."<<(int)span_addr.data[3];
	
	return ss.str();
}

/**
* Converts MAC address to string
**/
string network::MACToString(unsigned char * mac)
{
	stringstream ss;
	
	ss<<hex<<setw(2)<<setfill('0')<<(int)mac[0]<<":";
	ss<<hex<<setw(2)<<setfill('0')<<(int)mac[1]<<":";
	ss<<hex<<setw(2)<<setfill('0')<<(int)mac[2]<<":";
	ss<<hex<<setw(2)<<setfill('0')<<(int)mac[3]<<":";
	ss<<hex<<setw(2)<<setfill('0')<<(int)mac[4]<<":";
	ss<<hex<<setw(2)<<setfill('0')<<(int)mac[5];
	
	return ss.str();
}
