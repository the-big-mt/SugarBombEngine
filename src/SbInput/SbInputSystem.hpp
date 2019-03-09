/// @file

#pragma once

#include "SbInput/IInputSystem.hpp"

struct ICommon;

IInputSystem *CreateInputSystem(ICommon *apCommon);