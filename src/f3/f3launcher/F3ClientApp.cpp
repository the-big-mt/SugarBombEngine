/// @file

#include "F3ClientApp.hpp"

namespace f3bfg
{

void CClientApp::PostInit()
{
	mpCommon->Init(0, nullptr, lpCmdLine);
	
#if TEST_FPU_EXCEPTIONS != 0
	mpCommon->Printf(Sys_FPU_GetState());
#endif
};

void CClientApp::PostFrame()
{
	// run the game
	mpCommon->Frame();
};

}; // namespace f3bfg