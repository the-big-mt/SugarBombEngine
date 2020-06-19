/// @file

#pragma once

namespace sbe
{

struct IGame;

namespace SbGameFramework
{

class SbMapManager
{
public:
	SbMapManager(IGame &aGame);
	
	void LoadMap(const char *asFile, const char *asStartSpot);
	void UnloadMap();
	
	void ChangeMap(const char *asMap);
private:
	IGame &mGame;
};

};}; // namespace sbe::SbGameFramework