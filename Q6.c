//***********************************************************************************************************************************
// Q6: Function recieves a file name and a board, function reads from binary file into the board                                    *
//***********************************************************************************************************************************

#include "Header.h"

//Main question function
void LoadBoard(char *filename, Board board) //question 6
{
	unsigned short int currRow, currPlayer;
	unsigned short int player = 3; //In binary 11
	int col, row;

	FILE *bFile = fopen(filename, "rb");
	checkFile(bFile); //Open binary file and check opening

	for (row = 0; row < BOARD_SIZE; row++) //Going over all the rows
	{
		fread(&currRow, sizeof(unsigned short int), 1, bFile); //Reading a row into variable

		for (col = 7; col >= 0; col--) //Reading from left to right because of mirror effect
		{
			currPlayer = (player & currRow); //Inputting into current player the 'and' of player and row

			switch (currPlayer) 
			{
			case 0: //If current player is zero then write empty space into board
				board[row][col] = ' ';
				break;

			case 1: //If current player is 1 write T into board
				board[row][col] = 'T';
				break;

			case 2: //If current player is 2 in binary it is 10 so write B into board
				board[row][col] = 'B';
				break;
			}
			currRow >>= 2; //Shift bits two to the right because we used those already
		}
	}
	fclose(bFile); //Close file
}
