// playTicTacToe.cpp
// This set of functions are used to actually play the game.
// Play starts with the function: playTicTacToe() which is defined below

#include "nim.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;



std::vector<RockPile> initializeBoard(string test, int & totalRocks)
{
	int numPiles;
	
	std::string numPilesStr;

	// ADD CODE HERE TO CHECK THE BOUNDARIES


	numPilesStr = test.substr(0, 1);
	test.erase(0, 1);
	numPiles = stoi(numPilesStr);

	std::vector<RockPile> rp;

	rp.resize(numPiles);

	for (int i = 0; i < numPiles; i++)
	{
		rp[i].pileNum = i + 1;
		rp[i].numRocks = stoi(test.substr(0, 2));
		test.erase(0, 2);

		totalRocks += rp[i].numRocks;
	}
	//do {
	//	std::cout << "pile num: ";
	//	std::cin >> rp.pileNum;
	//	if (rp.pileNum < 3 || rp.pileNum > 9)
	//	{
	//		std::cout << "The number of rock piles must be between 3 and 9, please enter a new number..." << std::endl;
	//		pileGood = false;
	//	}
	//} while (pileGood != true);

	return rp;
}

void updateBoard(vector<RockPile> & rp, int move, int Player, int & totalRocks)
{
	int numRocks;
	int pile;

	string myPile;
	string myRocks;
	string isZero;
	char intStr[4];
	itoa(move, intStr, 10);
	string StrMove = string(intStr);

	isZero = StrMove.substr(1, 1);
	myPile = StrMove.substr(0, 1);

	if (isZero == "0")
	{
		myRocks = StrMove.substr(2, 1);
	}
	else
	{
		myRocks = StrMove.substr(1, 2);
	}

	pile = stoi(myPile);
	numRocks = stoi(myRocks);


	rp[pile - 1].numRocks = rp[pile - 1].numRocks - numRocks;
	totalRocks -= numRocks;
}

void displayBoard(vector<RockPile> rp)
{
	int numPiles;
	numPiles = rp.back().pileNum;

	std::cout << std::endl;
	std::cout << "Nim Board: " << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	for (int i = 0; i < numPiles; i++)
	{
		std::cout << "Rock Pile #" << i + 1 << " ->";
		for (int j = 0; j < rp[i].numRocks; j++)
		{
			std::cout << " *";
		}
		std::cout << "( " << rp[i].numRocks << ")";
		std::cout << " <- " << "Rock Pile #" << i + 1 << std::endl;
	}
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
}

int check4Win(int & totalRocks, bool myMove, int localPlayer, int opponent)
{
	int winner = noWinner;
	if (!myMove)
	{
		if (totalRocks  == 0)
		{
			if (totalRocks == 0) {
				winner = opponent;
			}
			else
			{
				winner = localPlayer;
			}
		}
	}

	else if (myMove)
	{
		if (totalRocks == 0)
		{
			if (totalRocks == 0) {
				winner = localPlayer;
			}
			else
			{
				winner = opponent;
			}
		}
	}
	else
	{
		winner = -5;
	}
	return winner;
}

int getMove(int numPiles, int Player)
{
	int move;
	string move_str;
	string pileString;
	int pileNumber;

	cout << "What Pile and How Many Rocks do you want to take? ";
	cin >> move_str;
	pileString = move_str.substr(0, 1);
	move = atoi(move_str.c_str());
	pileNumber = stoi(pileString);

	while (pileNumber < 3 || pileNumber > numPiles)
	{
		cout << "Incorrect pile selection. Please try again." << endl;
		cin >> move_str;
		pileString = move_str.substr(0, 1);
		move = atoi(move_str.c_str());
		pileNumber = stoi(pileString);

		// Check here to see if they can pick from that pile or not. if (board[move] == 'X' || board[move] == 'O') move = 0;
	}  // Change this to check boundaries of the piles.

	return move;
}

int playTicTacToe(SOCKET s, std::string serverName, std::string remoteIP, std::string remotePort, int localPlayer, char *test)
{
	// This function plays the game and returns the value: winner.  This value 
	// will be one of the following values: noWinner, xWinner, oWinner, TIE, ABORT
	int winner = noWinner;
	vector<RockPile> rp;
	int opponent;
	int move;
	int totalRocks = 0;
	bool myMove;
	int opponentMove;

	if (localPlayer == X_PLAYER) {
		std::cout << "Playing as player 1" << std::endl;
		opponent = O_PLAYER;
		myMove = true;
	}
	else {
		std::cout << "Playing as player 2" << std::endl;
		opponent = X_PLAYER;
		myMove = false;
	}

	rp = initializeBoard(test,totalRocks);
	displayBoard(rp);

	//int numSent = UDP_send(s, test, strlen(test) + 1, remoteIP.c_str(), remotePort.c_str());
	

	while (winner == noWinner) {
		if (myMove) {
			// Get my move & display board

			// Probably need to to put the while loop here to check for comments. 
			
			move = getMove(rp.size(), localPlayer);
			std::cout << "Board after your move:" << std::endl;
			updateBoard(rp, move, localPlayer,totalRocks);
			displayBoard(rp);
			std::string moveString = std::to_string(move);

			char moveMade[4];
			itoa(move, moveMade, 10);
			int numSent = UDP_send(s, moveMade, strlen(moveMade) + 1, remoteIP.c_str(), remotePort.c_str());
			winner = check4Win(totalRocks, myMove, localPlayer, opponent);
		}
		else {
			std::cout << "Waiting for your opponent's move..." << std::endl << std::endl;
			//Get opponent's move & display board

			int status = wait(s, WAIT_TIME, 0);
			if (status > 0) {


				char recvBuf[MAX_RECV_BUF];
				char remoteHost[v4AddressSize];
				char remote_Port[portNumberSize];
				int numRecv = UDP_recv(s, recvBuf, MAX_RECV_BUF - 1, remoteHost, remote_Port);
				    opponentMove = atoi(recvBuf);
				updateBoard(rp, opponentMove, opponent,totalRocks);
				displayBoard(rp);
				winner = check4Win(totalRocks, myMove, localPlayer, opponent);
			}
			else {
				winner = ABORT;
			}
		}
		myMove = !myMove;

		if (winner == ABORT) {
			std::cout << timestamp() << " - No response from opponent.  Aborting the game..." << std::endl;
		}
	
		if(winner != -5)
		{
			if (winner == localPlayer)
				std::cout << "You WIN!" << std::endl;
			else if (winner == opponent)
				std::cout << "I'm sorry.  You lost" << std::endl;
		}
	}

	return winner;
}