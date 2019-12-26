/// @file
/// @brief game server - a type of server which treat net clients/connections as players

#pragma once

#include <vector>

namespace sbe::SbGameFramework
{

class SbGameClient;
using tGameClientVec = std::vector<SbGameClient*>;

class SbGameServer
{
public:
	SbGameServer(uint32_t anPort);
	
	SbGameClient *GetClient(int anIndex) const;
private:
	tGameClientVec mvClients;
	
	INetServer *mpServer{nullptr};
};

}; // namespace sbe::SbGameFramework