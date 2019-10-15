/// @file

#include <windows.h>

#include "SbLibraryLoader.hpp"

namespace sbe
{

int SbLibraryLoader::Load(const char *asPath)
{
	return reinterpret_cast<int>(LoadLibrary(asPath));
};

void SbLibraryLoader::Unload(int anHandle)
{
	FreeLibrary(reinterpret_cast<HMODULE>(anHandle));
};

void *SbLibraryLoader::GetSymbol(int anHandle, const char *asSymbol)
{
	return reinterpret_cast<void *>(GetProcAddress(reinterpret_cast<HMODULE>(anHandle), asSymbol));
};

}; // namespace sbe