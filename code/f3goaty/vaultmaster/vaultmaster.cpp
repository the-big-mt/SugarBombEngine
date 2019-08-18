#include "Utils.hpp"
#include "MasterServer.hpp"
#include "../vaultmp.hpp"

void InputThread()
{
	char input[64];

	do
	{
		fgets(input, 64, stdin);

		if (strlen(input) > 0 && input[strlen(input) - 1] == '\n') input[strlen(input) - 1] = '\0';

	}
	while (strcmp(input, "exit") != 0);

	MasterServer::TerminateThread();
}

int main()
{
	printf("Vault-Tec MasterServer %s \n----------------------------------------------------------\n", MASTER_VERSION);

	Utils::timestamp();
	printf("Initializing RakNet...\n");

	std::thread hMasterThread = MasterServer::InitalizeRakNet();
	std::thread hInputThread = std::thread(InputThread);

	hMasterThread.join();

	if (hInputThread.joinable())
		hInputThread.join();

	return 0;
}