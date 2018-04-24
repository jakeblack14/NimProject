// playTicTacToe.cpp
// This set of functions are used to actually play the game.
// Play starts with the function: playTicTacToe() which is defined below

#include "nim.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

std::vector<RockPile> initializeBoard()
{
	int numPiles;
	int totalRocks = 0;
	std::string numPilesStr;
	std::string test = "50403030209";

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

void updateBoard( vector<RockPile> & rp, int move, int Player)
{
	int numRocks;
	int pile;
	
	string myPile;
	string myRocks;
	string isZero;
	char intStr[4];
	itoa(move,intStr, 10);
	string StrMove = string(intStr);

	isZero = StrMove.substr(1, 1);
	myPile = StrMove.substr(0, 1);

	if (isZero == "0")
	{
		myRocks = StrMove.substr(2, 1);
	}
	else
	{
		myRocks = StrMove.substr(1,2);
	}

	pile = stoi(myPile);
	numRocks = stoi(myRocks);


	rp[pile-1].numRocks = rp[pile-1].numRocks - numRocks;
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

int check4Win(char board[10])
{
	int winner = noWinner;

	// Check for vertical winners
	int i = 1;
	while (winner == noWinner && i < 4) {
		if (board[i] == board[i+3] && board[i] == board[i+6]) {
			winner = (board[i] == 'X') ? xWinner : oWinner;
		}
		i++;
	}

	// Check for horizontal winners
	i = 1;
	while (winner == noWinner && i < 8) {
		if (board[i] == board[i+1] && board[i] == board[i+2]) {
			winner = (board[i] == 'X') ? xWinner : oWinner;
		}
		i  += 3;
	}

	// Check for diagonal winners
	if (winner == noWinner) {
		if ( (board[1] == board[5] && board[1] == board[9]) ||
			 (board[3] == board[5] && board[3] == board[7]) ) {
			winner = (board[5] == 'X') ? xWinner : oWinner;
		}
	}

	// Check for tie
	i = 1;
	int numMoves = 0;
	while ( i < 10) {
		if ( (board[i] == 'X' || board[i] == 'O') ) {
			numMoves++;
		}
		i++;
	}
	if (winner == noWinner && numMoves == 9)
		winner = TIE;
	

	return winner;
}

int getMove(int numPiles, int Player)
{
	int move;
	string move_str;
	string pileString;
	int pileNumber;

	std::cout << "What Pile and How Many Rocks do you want to take? ";

	do {
		std::cin  >> move_str;
		pileString = move_str.substr(0, 1);
		move = atoi(move_str.c_str());
		pileNumber = stoi(pileString);

		
		// Check here to see if they can pick from that pile or not. if (board[move] == 'X' || board[move] == 'O') move = 0;
	} while (pileNumber < 1 || pileNumber > numPiles); // Change this to check boundaries of the piles.

	return move;
}

int playTicTacToe(SOCKET s, std::string serverName, std::string remoteIP, std::string remotePort, int localPlayer)
{
	// This function plays the game and returns the value: winner.  This value 
	// will be one of the following values: noWinner, xWinner, oWinner, TIE, ABORT
	int winner = noWinner;
	char board[10];
	int opponent;
	int move;
	bool myMove;
	vector<RockPile> rp;
	

	if (localPlayer == X_PLAYER) {
		std::cout << "Playing as X" << std::endl;
		opponent = O_PLAYER;
		myMove = true;
	} else {
		std::cout << "Playing as O" << std::endl;
		opponent = X_PLAYER;
		myMove = false;
	}

	rp = initializeBoard();
	displayBoard(rp);
	

	while (winner == noWinner) {
		if (myMove) {
			// Get my move & display board

		    // Probably need to to put the while loop here to check for comments. 

			move = getMove(rp.size(), localPlayer);
			std::cout << "Board after your move:" << std::endl;
			updateBoard(rp,move,localPlayer);
			displayBoard(rp);

			// Send move to opponent
/****			
	Task 1: "move" is an integer that was assigned a value (from 1 to 9) in the previous code segment.
	         Add code here to convert "move" to a null-terminated C-string and send it to your opponent at remoteIP using remotePort.
****/
			std::string moveString = std::to_string(move);

			char moveMade[4];
			itoa(move, moveMade, 10);
			int numSent = UDP_send(s, moveMade, strlen(moveMade)+1, remoteIP.c_str(), remotePort.c_str());
			
		} else {
			std::cout << "Waiting for your opponent's move..." << std::endl << std::endl;
			//Get opponent's move & display board
			int status = wait(s,WAIT_TIME,0);
			if (status > 0) {
				
			
				char recvBuf[MAX_RECV_BUF];
				char remoteHost[v4AddressSize];
				char remote_Port[portNumberSize];
				int numRecv = UDP_recv(s, recvBuf, MAX_RECV_BUF-1, remoteHost, remote_Port);
				int opponentMove = atoi(recvBuf);
				updateBoard(rp, opponentMove, opponent);
				displayBoard(rp);
/****			
Task 2: (i) Insert code inside this IF statement that will accept a null-terminated C-string from your
		opponent that represents their move.  Convert that string to an integer and then
		(ii) call a function that will update the game board (see above) using your opponent's move, and
		(iii) call a function that will display the updated board on your screen.
****/
			} else {
				winner = ABORT;
			}
		}
		myMove = !myMove;

		if (winner == ABORT) {
			std::cout << timestamp() << " - No response from opponent.  Aborting the game..." << std::endl;
		} else {
			winner = check4Win(board);
		}
		
		if (winner == localPlayer)
			std::cout << "You WIN!" << std::endl;
		else if (winner == TIE)
			std::cout << "It's a tie." << std::endl;
		else if (winner == opponent)
			std::cout << "I'm sorry.  You lost" << std::endl;
	}

	return winner;
}