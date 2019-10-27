//***********************************************************************************************************************************
// Q5: Function recieves a board and a file name, the function saves the board into the file in a binary representation             *
// 00 - Empty space, 01 - Player T, 10 - Player B. Saved row after row with no spaces from left to right.                           *
//***********************************************************************************************************************************

#include "Header.h"

//Main question function
void StoreBoard(Board board, char *filename)//question 5
{
	unsigned short int boardRow[BOARD_SIZE]; //Creating an array of short int to hold each row
	unsigned short int T = 1; //T is binary 01
	unsigned short int B = (T << 1); //B is binary 10
	unsigned int col, row; 

	FILE *bFile = fopen(filename, "wb"); 
	checkFile(bFile); //Open binary file and check opening 

	memset(boardRow, 0, sizeof(boardRow)); //Fill array with zeros to be empty spots

	for (row = 0; row < BOARD_SIZE; row++) //Going over all the rows
	{
		for (col = 0; col < BOARD_SIZE; col++) //Creating a row by columns 
		{
			switch (board[row][col]) //Cases of an empty square, player T or player B
			{
			case ' ': //In this case move the bits 2 right
				boardRow[row] <<= 2;
				break;

			case 'T': //In this case add T by shifting the bits 2 right and using or
				boardRow[row] <<= 2;
				boardRow[row] |= T;
				break;

			case 'B': //Same but for B
				boardRow[row] <<= 2;
				boardRow[row] |= B;
				break;
			}
		}
		fwrite(&boardRow[row], 1, sizeof(unsigned int short), bFile); //Writing the row to the binary file
	}
	fclose(bFile); //Close the binary file
}

//Function to check file opened succesfully
void checkFile(FILE *file)
{
	if (!file)
	{
		fprintf(stderr, "File opening failed \n");
		exit(STOP);
	}
}
