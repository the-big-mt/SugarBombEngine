/// @file

#pragma once

namespace sbe
{

struct IWindow
{
	struct Props
	{
		Props(const char *asTitle, int anWidth, int anHeight, bool abFullScreen)
			: msTitle(asTitle), mnWidth(anWidth), mnHeight(anHeight), mbFullScreen(abFullScreen){}
		
		const char *msTitle{""};

		int mnPosX{-1};
		int mnPosY{-1};

		int mnWidth{-1};
		int mnHeight{-1};

		bool mbFullScreen{false};
		bool mbBorder{false};
	};
};

}; // namespace sbe