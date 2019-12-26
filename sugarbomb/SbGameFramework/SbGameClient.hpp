/// @file

#pragma once

namespace sbe::SbGameFramework
{

class SbGameClient
{
public:
	SbGameClient();
	
	void SendMsg(const char *asMsg);
	
	void Disconnect(const char *asReason);
	
	bool IsLocal() const;
private:
	netadr_t mAdr{};
};

}; // namespace sbe::SbGameFramework