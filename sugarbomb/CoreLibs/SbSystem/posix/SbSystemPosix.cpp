/// @file

#include <dlfcn.h>

#include "SbSystem.hpp"

namespace sbe::SbSystem
{

int SbSystem::LoadLib(const char *asPath)
{
	return dlopen(asPath, RTLD_NOW);
};

void SbSystem::FreeLib(int anHandle)
{
	dlclose(anHandle);
};

void *SbSystem::GetLibSymbol(int anHandle, const char *asSymbol) const
{
	return dlsym(anHandle, asSymbol);
};

}; // namespace sbe::SbSystem