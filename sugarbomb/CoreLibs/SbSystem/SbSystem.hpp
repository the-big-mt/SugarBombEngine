/// @file

#pragma once

#include "CoreLibs/SbSystem/ISystem.hpp"

namespace sbe::SbSystem
{

class SbSystem : public ISystem
{
public:
	void Init() override;
	void Shutdown() override;
	
	int LoadLib(const char *asPath) override;
	void FreeLib(int anHandle) override;
	void *GetLibSymbol(int anHandle, const char *asSymbol) const override;
};

}; // sbe::SbSystem