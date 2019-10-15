/// @file

#pragma once

namespace sbe
{

struct ISystem
{
	///
	virtual void Init() = 0;
	
	///
	virtual void Shutdown() = 0;
	
	///
	virtual int LoadLib(const char *asPath) = 0;
	
	///
	virtual void FreeLib(int anHandle) = 0;
	
	///
	virtual void *GetLibSymbol(int anHandle, const char *asSymbol) const = 0;
	
	///
	template<typename T>
	T GetLibSymbol(int anHandle, const char *asSymbol) const {return reinterpret_cast<T>(GetLibSymbol(anHandle, asSymbol));}
};

}; // namespace sbe