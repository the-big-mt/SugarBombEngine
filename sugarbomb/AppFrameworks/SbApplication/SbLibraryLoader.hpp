/// @file

#pragma once

namespace sbe
{

class SbLibraryLoader
{
public:
	///
	static int Load(const char *asPath);
	
	///
	static void Unload(int anHandle);
	
	///
	static void *GetSymbol(int anHandle, const char *asSymbol);
	
	///
	template<typename T>
	inline static T GetSymbol(int anHandle, const char *asSymbol){return reinterpret_cast<T>(GetSymbol(anHandle, asSymbol));}
};

}; // namespace sbe