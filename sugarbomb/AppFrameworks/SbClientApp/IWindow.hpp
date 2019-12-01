/// @file

#pragma once

namespace sbe
{

struct IWindow
{
	struct Props
	{
		const char *asTitle{""};

		int mnPosX{-1};
		int mnPosY{-1};

		int mnWidth{-1};
		int mnHeight{-1};

		bool mbFullScreen{false};
		bool mbBorder{false};
	};
};

}; // namespace sbe