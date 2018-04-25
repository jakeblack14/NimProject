// nim.h    Header file for Nim project
#pragma once
#include <winsock2.h>
#include <string>

#define NIM_QUERY	"Who?"
#define NIM_NAME	"Name="
#define NIM_CHALLENGE	"Player="

static char NIM_UDPPORT[] = "29333";
const int v4AddressSize = 16;
const int portNumberSize = 10;
const int WAIT_TIME = 120;
const int MAX_SEND_BUF = 2048;
const int MAX_RECV_BUF = 2048;
const int MAX_SERVERS = 100;
const int MAX_NAME = 100;
const int MAX_INTERFACES = 20;
const int X_PLAYER = 1;
const int O_PLAYER = 2;
const int ABORT = -1;
const int noWinner = -5;
const int xWinner = 1;
const int oWinner = 2;
const int TIE = 3;

struct ServerStruct {
	std::string name;
	std::string host;
	std::string port;
};

struct RockPile {
	int pileNum;
	int numRocks;
};

void playNim(SOCKET, bool, char*, bool, bool, char*, char*, bool);
SOCKET connectsock(const char*, const char*, const char*);
SOCKET passivesock(const char*, const char*);
int UDP_recv(SOCKET, char*, int, char*, char*);
int UDP_send(SOCKET, char*, int, const char*, const char*);
int wait(SOCKET, int, int);
char* timestamp();
int getServers(SOCKET, char*, char*, ServerStruct[]);
int playTicTacToe(SOCKET, std::string, std::string, std::string, int, char*);
int serverMain(int, char *argv[], std::string);
int clientMain(int, char *argv[], std::string);
int getIPAddressInfo(char*, char*);
