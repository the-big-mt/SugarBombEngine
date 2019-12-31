/*
*******************************************************************************

Copyright (C) 2019 SugarBombEngine Developers

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

float SbAbs(float afValue);

float SbSin(float afRadians);
float SbCos(float afRadians);

float SbASin(float afValue);
float SbACos(float afValue);
float SbATan(float afValue);

template<typename T>
T SbMin(T aTA, T aTB)
{
	return std::min(aTA, aTB);
};

template<typename T>
T SbMax(T aTA, T aTB)
{
	return std::max(aTA, aTB);
};

template<typename T>
T SbClamp(T aTValue, T aTMin, T aTMax)
{
	return std::clamp(aTValue, aTMin, aTMax);
};

}; // namespace sbe