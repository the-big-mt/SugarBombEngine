/// @file

#pragma once

#include "SbNetwork/INetworkSystem.hpp"

class CNetworkSystem : public INetworkSystem
{
public:
	void Init() override;
	void Shutdown() override;
	
	int GetLocalIPCount() const override;
	const char *GetLocalIP(int anIndex) const override;
};