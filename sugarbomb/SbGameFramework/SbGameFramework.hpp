/// @file

#pragma once

#include <functional>

#include "SbGameFramework/IGameFramework.hpp"

namespace sbe
{

struct ISystem;
struct IGame;

namespace SbGameFramework
{

class SbGameFramework : public IGameFramework
{
public:
	SbGameFramework(ISystem &aSystem);
	
	void Init() override;
	void Shutdown() override;
private:
	void LoadGameModule();
	void UnloadGameModule();
	
	std::reference_wrapper<ISystem> mSystem;
	
	IGame *mpGame{nullptr};
	
	int mnGameLib{-1};
};

};}; // namespace sbe::SbGameFramework