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

#include <cmath>

#include "CoreLibs/SbMain/SbMath.hpp"

//*****************************************************************************

namespace sbe
{

float SbAbs(float afValue)
{
	return std::abs(afValue);
};

float SbSin(float afRadians)
{
	return std::sin(afRadians);
};

float SbCos(float afRadians)
{
	return std::cos(afRadians);
};

float SbASin(float afValue)
{
	return std::asin(afValue);
};

float SbACos(float afValue)
{
	return std::acos(afValue);
};

float SbATan(float afValue)
{
	return std::atan(afValue);
};

}; // namespace sbe