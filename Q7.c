//***********************************************************************************************************************************
// Karina Batmanishvili                                                                                                             *
// Id: 321800898                                                                                                                    *
// Q7: Function recieves a board and a starting player, function plays a game where the computer decides on the moves both          *
// top player and down player (computer plays a game on its own) function prints the board it recieved and on every turn will       *
// print the player who's turn it is now and the gameplay he made                                                                   *
//***********************************************************************************************************************************

#include "Header.h"

////////////////////////////////////////////////static functions/////////////////////////////////////////////////////////
static BOOL gameOn           (Board board);
static BOOL checkForFinisher (Player player, unsigned int row, Board board);
static BOOL IsChanged        (Board board1, Board board2);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Main question function
void PlayGame(Board board, Player starting_player) //question 7
{
	Player player = starting_player;
	BOOL game = TRUE;

	printf("Game begins!\n");
	PrintBoard(board); 

	while (gameOn(board)) //Checks for possible end game ///need to check when board is non changed then game over
	{//Checking player turn and printing who's turn it is, and sending to function to play a single turn (question 4)

		if (player == 'T')
		{
			printf("Player TOP_DOWN's turn:\n");
			Turn(board, player);
			player = 'B';
		}

		else
		{
			printf("Player BOTTOM_UP's turn:\n");
			Turn(board, player);
			player = 'T';
		}		
	}

	printf("END GAME\n");
}

void PrintBoard(Board board) //Function to print board
{
	unsigned int col, row;
	unsigned char rowLetter;

	printf("+---+---+---+---+---+---+---+---+---+\n");
	printf("|   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |\n");
	printf("+---+---+---+---+---+---+---+---+---+\n");

	for (row = 0; row < BOARD_SIZE; row++)
	{
		rowLetter = row + 'A';
		printf("| %C |", rowLetter);

		for (col = 0; col < BOARD_SIZE; col++)
			printf(" %C |", board[row][col]);
		
		printf("\n");
		printf("+---+---+---+---+---+---+---+---+---+\n");
	}
}

static BOOL gameOn(Board board) //returns false if endgame
{
	BOOL game = TRUE;
	unsigned int col, row, countT = 0, countB = 0;

	for (row = 0; row < BOARD_SIZE; row++)
	{
		//Checking for player in finish line
		if (row == 0 && game) //If bottom player reached top row
			game = checkForFinisher('B', row, board);
		else if (row == 7 && game) //If top player reached bottom row
			game = checkForFinisher('T', row, board);

		//Counting the amount of players left
		for (col = 0; col < BOARD_SIZE && game; col++)
		{
			if (board[row][col] == 'T')
				countT++;
			else if (board[row][col] == 'B')
				countB++;
		}
	}

	if (!countT || !countB) //If one of the players is out of game pieces end game ensued
		game = FALSE;

	return game;
}

static BOOL checkForFinisher(Player player, unsigned int row, Board board) //returns false if endgame
{
	BOOL game = TRUE;
	unsigned int col;

	for (col = 0; col < BOARD_SIZE; col++) //Goes over the columns of given row and checks for the player recieved if found game ends
		if (board[row][col] == player)
			game = FALSE;

	return game;
}