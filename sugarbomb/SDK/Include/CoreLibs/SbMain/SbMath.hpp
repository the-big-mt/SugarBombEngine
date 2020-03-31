/*
*******************************************************************************

Copyright (C) 2019-2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

#include <algorithm>

//*****************************************************************************

namespace sbe
{

class SbMath
{
public:
	static float Abs(float afValue);

	static float Sin(float afRadians);
	static float Cos(float afRadians);

	static float ASin(float afValue);
	static float ACos(float afValue);
	static float ATan(float afValue);

	template<typename T>
	static T Min(T aTA, T aTB)
	{
		return std::min(aTA, aTB);
	};

	template<typename T>
	static T Max(T aTA, T aTB)
	{
		return std::max(aTA, aTB);
	};

	template<typename T>
	static T Clamp(T aTValue, T aTMin, T aTMax)
	{
		return std::clamp(aTValue, aTMin, aTMax);
	};
};

}; // namespace sbe