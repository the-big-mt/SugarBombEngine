/// @file

#include "F3ClientApp.hpp"

int main(int argc, const char **argv)
{
	f3bfg::CClientApp App(argc, argv);
	
	App.Run();

	// never gets here
	return EXIT_SUCCESS;
};