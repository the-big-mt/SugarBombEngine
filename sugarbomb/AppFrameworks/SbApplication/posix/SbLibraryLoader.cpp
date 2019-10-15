/// @file

#include <dlfcn.h>

#include "SbLibraryLoader.hpp"

namespace sbe
{

int SbLibraryLoader::Load(const char *asPath)
{
	return dlopen(asPath, RTLD_NOW);
};

void SbLibraryLoader::Unload(int anHandle)
{
	dlclose(anHandle);
};

void *SbLibraryLoader::GetSymbol(int anHandle, const char *asSymbol)
{
	return dlsym(anHandle, asSymbol);
};

}; // namespace sbe