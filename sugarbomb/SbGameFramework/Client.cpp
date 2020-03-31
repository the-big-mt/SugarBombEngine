/*
*******************************************************************************

Copyright (C) 2019-2020 SugarBombEngine Developers

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

This file incorporates work originally covered by the MIT License. See
LICENSE-vaultmp file for details.

*******************************************************************************
*/
#include "Client.hpp"

#include <algorithm>

using namespace RakNet;

Guarded<> Client::cs;
std::map<RakNetGUID, Client*> Client::clients;
std::stack<unsigned int> Client::clientID;

Client::Client(RakNetGUID guid, NetworkID player) : guid(guid), player(player)
{
	cs.Operate([guid, this]() {
		ID = clientID.top();
		clients.emplace(guid, this);
		clientID.pop();
	});
}

Client::~Client()
{
	cs.Operate([this]() {
		clients.erase(this->guid);
		clientID.push(this->ID);
	});
}

void Client::SetMaximumClients(unsigned int clients)
{
	cs.Operate([clients]() {
		while (!clientID.empty())
			clientID.pop();

		for (signed int i = clients - 1; i >= 0; --i)
			clientID.push(i);
	});
}

unsigned int Client::GetClientCount()
{
	return cs.Operate([]() {
		return clients.size();
	});
}

Client* Client::GetClientFromGUID(RakNetGUID guid)
{
	return cs.Operate([guid]() -> Client* {
		auto it = clients.find(guid);

		if (it != clients.end())
			return it->second;

		return nullptr;
	});
}

Client* Client::GetClientFromID(unsigned int ID)
{
	return cs.Operate([ID]() -> Client* {
		for (auto it = clients.begin(); it != clients.end(); ++it)
			if (it->second->GetID() == ID)
				return it->second;

		return nullptr;
	});
}

Client* Client::GetClientFromPlayer(NetworkID id)
{
	return cs.Operate([id]() -> Client* {
		for (auto it = clients.begin(); it != clients.end(); ++it)
			if (it->second->GetPlayer() == id)
				return it->second;

		return nullptr;
	});
}

std::vector<RakNetGUID> Client::GetNetworkList(Client* except)
{
	return cs.Operate([except]() {
		std::vector<RakNetGUID> network;

		for (auto it = clients.begin(); it != clients.end(); ++it)
			if (it->second != except)
				network.emplace_back(it->first);

		return network;
	});
}

std::vector<RakNetGUID> Client::GetNetworkList(RakNetGUID except)
{
	return cs.Operate([except]() {
		std::vector<RakNetGUID> network;

		for (auto it = clients.begin(); it != clients.end(); ++it)
			if (it->first != except)
				network.emplace_back(it->first);

		return network;
	});
}

std::vector<RakNetGUID> Client::GetNetworkList(const std::vector<NetworkID>& players, RakNetGUID except)
{
	return cs.Operate([&players, except]() {
		std::vector<RakNetGUID> network;

		for (auto it = clients.begin(); it != clients.end(); ++it)
			if (it->first != except && find(players.begin(), players.end(), it->second->player) != players.end())
				network.emplace_back(it->first);

		return network;
	});
}

RakNetGUID Client::GetGUID()
{
	return guid;
}

unsigned int Client::GetID()
{
	return ID;
}

NetworkID Client::GetPlayer()
{
	return player;
}
