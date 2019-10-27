//***********************************************************************************************************************************
// Karina Batmanishvili                                                                                                             *
// Id: 321800898                                                                                                                    *
// Q1: Function recieves a board with the current gameplay and a position in the board,                                             *
// if theres a player in that position function creates a binary tree of all possible moves, otherwise return is NULL               *
//***********************************************************************************************************************************

#include "Header.h"

////////////////////////////////////////////////////////////////////////////////static functions///////////////////////////////////////////////////////////////////////////////////
static SingleSourceMovesTreeNode *FindSingleSourceMovesRec (Board board, checkersPos currPos, int row, int col, unsigned short count, int side, int mark); //Recursive function 
static SingleSourceMovesTreeNode *createTreeNode           (checkersPos src, unsigned short count, Board board); //Creates a single node
static BOOL                       checkColRow              (int plc); //Checking for both row and column to not excceed the eight allowed
static void                       copyBoard                (Board board, Board newBoard); //Copies the board entirely
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Main question function
SingleSourceMovesTree *FindSingleSourceMoves(Board board, checkersPos *src) //question 1
{
	unsigned short count = 0; //Setting counter at zero for captures
	int row = (int)(src->row - 'A'); //Casting for easier use both row and column
	int col = (int)(src->col - '0');
	
	SingleSourceMovesTree *res = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree)); //Creating the tree we will return in the end
	checkMalloc(res);

	Player Player = board[row][col]; //Setting the player to be the character that is in the board in the place of row and column
	int mark = checkRange(Player); //Creating a mark either one or minus one to simplify next functions for moving the rows accordingly to player
	///MARK NEVER CHANGES
	
	if (Player == ' ') //If there's no player in the current position return is NULL
		return NULL;

	else //If player is found in the position
	{
		res->source = createTreeNode(*src, count, board);  //Creating the root, board of the root is the same board recieved and position is the one recieved, count iz zero 

		//Creating the left and right tree, checking we aren't going over the board before entering recursion
		if (checkColRow(row + mark) && checkColRow(col - 1)) //Left, going left is column minus one
			res->source->next_move[LEFT] = FindSingleSourceMovesRec(board, *src, row + mark, col - 1, count, LEFT-1, mark); //Left tree
		if (checkColRow(row + mark) && checkColRow(col + 1)) //Right, going right is adding one to column
			res->source->next_move[RIGHT] = FindSingleSourceMovesRec(board, *src, row + mark, col + 1, count, RIGHT, mark); //Right tree
	}
	
	return res; //Return the tree we created
}

//Recursive function to return root of tree, recieves current board, current position in board, simplifies row and column, count of captures, the side we are moving to (left or right) and mark of the player for moving the rows
static SingleSourceMovesTreeNode *FindSingleSourceMovesRec(Board board, checkersPos currPos, int row, int col, unsigned short count, int side, int mark)
{
	SingleSourceMovesTreeNode *node = NULL; //Node will be root of the tree we will return
	checkersPos nextPos;

	nextPos.row = (char)(row + 'A'); //Creating the new position
	nextPos.col = (char)(col + '0');

	if (checkRange(board[row][col]) == mark) //We check to see if next place is the same player, if it is return NULL because this path is bad, return is NULL
		return node;

	else if (count == 0 && board[row][col] == ' ') //If we encounter an empty space and we didn't make a capture it's a valid move
	{
		node = createTreeNode(nextPos, count, board); //Inputting the information into the node that wil be a leaf
		swapInBoard(node->board, currPos, nextPos); //Swapping the player position from current to next
	}

	else if (count == 0 && checkRange(board[row][col]) != mark && checkRange(board[row][col]) != 0) //Possible FIRST capture, the item we are on currently is the opposite player
	{
		//Since it's a capture, next position is one after the capture
		nextPos.col += side;
		nextPos.row += mark;
		
		if (checkColRow(nextPos.col - '0') && checkColRow(nextPos.row - 'A') && board[row + mark][col + side] == ' ')  //Making sure next position is a valid board position and is empty 
		{
			node = createTreeNode(nextPos, count + 1, board); //Creating the node with the next position, the count is growing because of the capture and the board is the same one
			node->board[row][col] = ' '; //Capturing the player inbetween 
			swapInBoard(node->board, currPos, nextPos); //Swapping player position

			row += mark;
			col += side;

			//Creating left and right trees
			if (checkColRow(row + mark) && checkColRow(col - 1))  
				node->next_move[LEFT] = FindSingleSourceMovesRec(node->board, nextPos, row + mark, col - 1, count + 1, LEFT - 1, mark);
			if (checkColRow(row + mark) && checkColRow(col + 1))
				node->next_move[RIGHT] = FindSingleSourceMovesRec(node->board, nextPos, row + mark, col + 1, count + 1, RIGHT, mark);
		}
	}

	else //if (count != 0) enter this condition only if we already have a capture
	{
		//Since it's a capture, next position is one after the capture
		nextPos.col += side;
		nextPos.row += mark;

		int possibleCapture = checkRange(board[row][col]); 
		//Here we make sure next position is valid board move and also we make sure it's a capture by checking the item inbetween the positions to be the opposite player (checking by mark)
		if (checkColRow(nextPos.col - '0') && checkColRow(nextPos.row - 'A') && possibleCapture != mark && possibleCapture != 0 && board[row + mark][col + side] == ' ')
		{
			node = createTreeNode(nextPos, count + 1, board); //If it upholds condition then create a node, capture grows one more
			node->board[row + mark][col + side] = ' '; //Capturing the player inbetween
			swapInBoard(node->board, currPos, nextPos); // Swapping player position

			//Creating left and right trees
			if (checkColRow(row + 2*mark) && checkColRow(col - 2)) 
				node->next_move[LEFT] = FindSingleSourceMovesRec(node->board, nextPos, row + 2*mark, col - 2, count + 1, LEFT - 1, mark);
			if (checkColRow(row + 2*mark) && checkColRow(col + 2))
				node->next_move[RIGHT] = FindSingleSourceMovesRec(node->board, nextPos, row + 2*mark, col + 2, count + 1, RIGHT, mark);
		}
	}
	return node;
}

//Function creates a single node
static SingleSourceMovesTreeNode *createTreeNode(checkersPos src, unsigned short count, Board board)
{
	SingleSourceMovesTreeNode *res = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
	checkMalloc(res); //Allocating memory for the node and checking it

	res->pos = (checkersPos*)malloc(sizeof(checkersPos));
	checkMalloc(res->pos); //Allocating memory for position and checking it

	*(res->pos) = src; //Inputting the current position into node
	copyBoard(board, res->board); //Copying the board into the node
	res->total_captures_so_far = count; //Inputting capture count into the node
	res->next_move[LEFT] = res->next_move[RIGHT] = NULL; //Setting the pointers to left and right of root to be NULL

	return res;
}

//Function to swap two items in board by positions
void swapInBoard(Board board, checkersPos curr, checkersPos next)
{
	unsigned int currRow, currCol, nextRow, nextCol; //Variables for readibility

	currRow = (unsigned int)(curr.row - 'A');
	currCol = (unsigned int)(curr.col - '0');
	nextRow = (unsigned int)(next.row - 'A');
	nextCol = (unsigned int)(next.col - '0');

	Player playerInCurr = board[currRow][currCol]; //Saving the character in the current position

	board[currRow][currCol] = board[nextRow][nextCol]; //Inputting the character in next to current position in board
	board[nextRow][nextCol] = playerInCurr; //Inputting the character that was in current position into next position
}

static void copyBoard(Board board, Board newBoard) //Copies the board entirely using loop for rows and loop for columns
{
	unsigned int row, col;

	for (row = 0; row < BOARD_SIZE; row++)
		for (col = 0; col < BOARD_SIZE; col++)
			newBoard[row][col] = board[row][col];
}

static BOOL checkColRow(int plc) //Function returns TRUE if item recieved is a value between zero to seven which is our boards possible values
{
	return (plc >= 0 && plc < 8);
}

int checkRange(Player player) //Function returns a mark to use for moving in the rows according to player
{
	int mark;

	if (player == 'B') //For bottom we go up in the rows so mark is minus one
		mark = -1;

	else if (player == 'T') //For top we move down so mark is one
		mark = 1;

	else // In any other case mark will be zero
		mark = 0;

	return mark;
}

void checkMalloc(void *pointer) //Function to check memory allocation
{
	if (!pointer)
	{
		fprintf(stderr, "Allocation failed \n");
		exit(STOP);
	}
}