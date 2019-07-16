#ifdef __WIN32__
#include <winsock2.h>
#endif

#include <cstdio>
#include <ctime>
#include <map>
#include <thread>
#include <chrono>

#include "RakNet.hpp"

#include "../ServerEntry.hpp"
#include "Data.hpp"
#include "Utils.hpp"

#define RAKNET_PORT        1660
#define RAKNET_CONNECTIONS 128

typedef std::map<RakNet::SystemAddress, RakNet::ServerEntry> ServerMap;

class MasterServer
{

	private:
		static RakNet::RakPeerInterface* peer;
		static RakNet::SocketDescriptor* sockdescr;

		static ServerMap serverList;
		static void RemoveServer(RakNet::SystemAddress addr);

		static void MasterThread();
		static bool thread;

	public:
		static std::thread InitalizeRakNet();
		static void TerminateThread();

};
