/// @file

#include "SbClientApp.hpp"

int main(int argc, const char **argv)
{
	SbClientApp App(argc, argv);
	
	App.Run();

	// never gets here
	return EXIT_SUCCESS;
};