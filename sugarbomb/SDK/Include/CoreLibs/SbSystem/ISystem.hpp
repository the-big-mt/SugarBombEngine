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
};

}; // namespace sbe