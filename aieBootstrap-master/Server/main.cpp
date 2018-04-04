#include <iostream>
#include <string>

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include "GameMessages.h"

void handleNetworkMessages(RakNet::RakPeerInterface* pPeerInterface);

int main()
{
	const unsigned short PORT = 5456;
	RakNet::RakPeerInterface* pPeerInterface = nullptr;

	//startup the server, and stat it listening to clients
	std::cout << "Starting up the server...\n";

	//initialise the raknet peer interface first
	pPeerInterface = RakNet::RakPeerInterface::GetInstance();

	//create a socket descriptor to describe this connection
	RakNet::SocketDescriptor sd(PORT, 0);

	//now call startup - max of 32 connections, on the assinged port
	pPeerInterface->Startup(32, &sd, 1);
	pPeerInterface->SetMaximumIncomingConnections(32);

	handleNetworkMessages(pPeerInterface);
	sendClientPing(pPeerInterface);

	return 0;
}

void handleNetworkMessages(RakNet::RakPeerInterface* pPeerInterface)
{
	RakNet::Packet* packet = nullptr;

	while (true)
	{
		for (packet = pPeerInterface->Receive(); packet;
			pPeerInterface->DeallocatePacket(packet),
			packet = pPeerInterface->Receive())
		{
			switch (packet->data[0])
			{
			case ID_NEW_INCOMING_CONNECTION:
				std::cout << "A new connection is incoming.\n";
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				std::cout << "A client has disconnected.\n";
				break;
			case ID_CONNECTION_LOST:
				std::cout << "A client has lost connection.\n";
				break;
			default:
				std::cout << "Received a message with an unkown id: " << packet->data[0];
				break;
			}
		}
	}
}