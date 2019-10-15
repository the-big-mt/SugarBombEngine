/// @file

#include <windows.h>

#include "SbSystem.hpp"

namespace sbe::SbSystem
{

int SbSystem::LoadLib(const char *asPath)
{
	return reinterpret_cast<int>(LoadLibrary(asPath));
};

void SbSystem::FreeLib(int anHandle)
{
	FreeLibrary(reinterpret_cast<HMODULE>(anHandle));
};

void *SbSystem::GetLibSymbol(int anHandle, const char *asSymbol) const
{
	return reinterpret_cast<void *>(GetProcAddress(reinterpret_cast<HMODULE>(anHandle), asSymbol));
};

}; // namespace sbe::SbSystem