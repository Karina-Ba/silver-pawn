//***********************************************************************************************************************************
// Karina Batmanishvili                                                                                                             *
// Id: 321800898                                                                                                                    *
// Q3: Function recieves a board and a player and returns a list of lists with best possible moves from                             *
// every position of the player recieved and has a possible move to make                                                            *
//***********************************************************************************************************************************

#include "Header.h"

////////////////////////////////////////////////static functions/////////////////////////////////////////////////////////
static void addListToList (MultipleSourcesMovesList *mult_list, SingleSourceMovesList *src_list);
static void FreeTree      (SingleSourceMovesTree *tree); 
static void FreeTreeRec   (SingleSourceMovesTreeNode *root);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Main question function
MultipleSourcesMovesList *FindAllPossiblePlayerMoves(Board board, Player player) //question 3
{
	MultipleSourcesMovesList *res = (MultipleSourcesMovesList*)malloc(sizeof(MultipleSourcesMovesList)); 
	checkMalloc(res); //Allocating memory for list and checking the allocation
	res->head = res->tail = NULL; //Setting the list to be empty

	checkersPos source;
	SingleSourceMovesTree *src_tree;
	SingleSourceMovesList *src_list;
	unsigned int row, col;

	//Loop to go through the board and find any and every position with the player given, if a position as such is found create a source tree and from that tree create an optimal moves list
	//then input it to the possible player moves result list
	for (row = 0; row < BOARD_SIZE; row++)
	{
		for (col = 0; col < BOARD_SIZE; col++)
		{
			if (board[row][col] == player)
			{
				source.row = (char)(row + 'A'); //Creating current position in char
				source.col = (char)(col + '0');

				src_tree = FindSingleSourceMoves(board, &source); //Creating a tree with possible moves from current position (question1)
				if (src_tree->source->next_move[RIGHT] || src_tree->source->next_move[LEFT]) //If the tree has possible gameplay only, input it into list (this also makes sure source list isn't empty)
				{
					src_list = FindSingleSourceOptimalMove(src_tree); //Creating a list that is the optimal move from the tree (question2)
					addListToList(res, src_list); //Adding the list to the result list (list of lists of optimal moves)
				}

				FreeTree(src_tree); //Freeing the tree since we have no use for it anymore
			}
		}
	}

	return res;
}

//Function to add a single moves list to the bigger list of all possible moves
static void addListToList(MultipleSourcesMovesList *mult_list, SingleSourceMovesList *src_list)
{
	MultipleSourceMovesListCell *cell = (MultipleSourceMovesListCell*)malloc(sizeof(MultipleSourceMovesListCell)); 
	checkMalloc(cell); //Allocating memory and checking allocation

	cell->single_source_moves_list = src_list; //Creating a list cell using the single source moves list recieved
	cell->next = NULL;

	if (!mult_list->head) //If list is empty input cell as both head and tail 
		mult_list->head = mult_list->tail = cell;
	
	else //If list is not empty, add cell to end of list
	{
		mult_list->tail->next = cell;
		mult_list->tail = cell;
	}
}

//Function to free the tree
static void FreeTree(SingleSourceMovesTree *tree)
{
	FreeTreeRec(tree->source); //Recursively free the nodes
	free(tree);
}

//Recursive function to free the tree, from left and right nodes, then position then the node itself
static void FreeTreeRec(SingleSourceMovesTreeNode *root)
{
	if (root != NULL)
	{
		if(root->next_move[LEFT])
			FreeTreeRec(root->next_move[LEFT]);
		if(root->next_move[RIGHT])
			FreeTreeRec(root->next_move[RIGHT]);
		free(root->pos);
		free(root);
	}
}