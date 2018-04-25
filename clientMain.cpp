// clientMain.cpp
//   This function serves as the "main" function for the client-side
#include "nim.h"
#include <iostream>
#include <string>
#include <WinSock2.h>

int clientMain(int argc, char *argv[], std::string playerName)
{
	char yes[MAX_SEND_BUF] = "YES";
	char no[MAX_SEND_BUF] = "NO";
	char great[MAX_SEND_BUF] = "Great!";
	char board[MAX_RECV_BUF];
	std::string host;
	std::string port;
	ServerStruct serverArray[MAX_SERVERS];
	char received[MAX_RECV_BUF];

	SOCKET s = connectsock("","","udp");	// Create a socket  (Don't need to designate a host or port for UDP)

	// Find all TicTacToe servers on our subnet
	std::cout << std::endl << "Looking for Nim servers ... " << std::endl;

	char broadcastAddress[v4AddressSize];
	char myIPAddress[v4AddressSize];
	int status = getIPAddressInfo(myIPAddress, broadcastAddress);
	if (status != 0) {
		std::cerr << "No interface found that supports broadcasting." << std::endl;
		return -1;
	}
	int numServers = getServers(s, broadcastAddress, NIM_UDPPORT, serverArray);

	if (numServers == 0) {
		std::cout << std::endl << "Sorry.  No Nim servers were found.  Try again later." << std::endl << std::endl;
	} else {
		// Display the list of servers found
		std::cout << std::endl << "Found Nim server";
		if (numServers == 1) {
			std::cout << ":" << "  " << serverArray[0].name << std::endl;
		} else {
			std::cout << "s:" << std::endl;
			for (int i=0; i<numServers; i++) {
				std::cout << "  " << i+1 << " - " << serverArray[i].name << std::endl;
			}
			std::cout << std::endl << "  " << numServers+1 << " - QUIT" << std::endl;
		}
		std::cout << std::endl;

		// Allow user to select someone to challenge
		int answer = 0;
		std::string answerStr;
		if (numServers == 1) {
			std::cout << "Do you want to challenge " << serverArray[0].name << "? ";
			std::getline(std::cin, answerStr);
			if (answerStr[0] == 'y' || answerStr[0] == 'Y') answer = 1;
		} else if (numServers > 1) {
			std::cout << "Who would you like to challenge (1-" << numServers+1 << ")? ";
			std::getline(std::cin,answerStr);
			answer = atoi(answerStr.c_str());
			if (answer > numServers) answer = 0;
		}
			
		if (answer >= 1 && answer <= numServers) {
			// Extract the opponent's info from the server[] array
			std::string serverName; 
			serverName = serverArray[answer-1].name;		// Adjust for 0-based array
			host = serverArray[answer-1].host;
			port = serverArray[answer-1].port;

			// Append playerName to the NIM_CHALLENGE string & send a challenge to host:port
			char buffer[MAX_SEND_BUF];
			strcpy_s(buffer, NIM_CHALLENGE);
			strcat_s(buffer,playerName.c_str());
			int len = UDP_send(s, buffer, strlen(buffer)+1,(char*)host.c_str(), (char*)port.c_str());
			len = UDP_recv(s, received, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
			if (received[0] == 'y' || received[0] == 'Y')
			{
				UDP_send(s, great, strlen(great) + 1, (char*)host.c_str(), (char*)port.c_str());
				// Play the game.  You are the 'X' player

				int len = UDP_recv(s, board, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());

				int winner = playTicTacToe(s, serverName, host, port, X_PLAYER, board);
			}
		}
	}

	closesocket(s);
	return 0;
}