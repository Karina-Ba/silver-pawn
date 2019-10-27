//***********************************************************************************************************************************
// Karina Batmanishvili                                                                                                             *
// Id: 321800898                                                                                                                    *
// Q4: Function recieves a board and a player and makes the best move from possible moves in current board,                         *
// in case of captures it takes the captured off the board                                                                          *
//***********************************************************************************************************************************

#include "Header.h"

////////////////////////////////////////////////static functions/////////////////////////////////////////////////////////////
static SingleSourceMovesList *FindBestPossibleMove (MultipleSourcesMovesList *all_possible_moves);
static void                   ChangeBoard          (Board board, Player player, SingleSourceMovesList *best_possible_move);
static void                   FreeMultiList        (MultipleSourcesMovesList *List_of_lists);
static void                   freeList             (SingleSourceMovesList *list);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Main question function
void Turn(Board board, Player player) //question 4
{
	MultipleSourcesMovesList *all_possible_moves = FindAllPossiblePlayerMoves(board, player); //Creating list of all possible moves for current player (question 3)
	
	if (!all_possible_moves->head)
	{
		printf("No possible moves\nEnd Game\n");
		exit(STOP);
	}

	SingleSourceMovesList *best_possible_move = FindBestPossibleMove(all_possible_moves); //Choosing the optimal move from the list

	printf("%C%C -> %C%C \n", best_possible_move->head->position->col + 1, best_possible_move->head->position->row, best_possible_move->tail->position->col + 1, best_possible_move->tail->position->row); //Printing the move made

	ChangeBoard(board, player, best_possible_move); //Changing the board
	
	FreeMultiList(all_possible_moves); //Freeing the list
}

//Function to find optimal move out of all possible existing moves on the board
static SingleSourceMovesList *FindBestPossibleMove(MultipleSourcesMovesList *all_possible_moves)
{
	MultipleSourceMovesListCell *curr = all_possible_moves->head; //Creating a cell to go through the list
	MultipleSourceMovesListCell *best_move = all_possible_moves->head; //Creating a cell to hold best option
	unsigned short most_captures = best_move->single_source_moves_list->head->captures; //Inputting a variable of captures for comparison 

	while (curr) //While there's still a list cell
	{
		if (curr->single_source_moves_list->tail->captures >= most_captures) //Compare the current max captures with the current cell in the list
		{ //If it's bigger than current max then change the current max captures and input the current cell as optimal move
			most_captures = curr->single_source_moves_list->tail->captures;
			best_move = curr;
		}
		curr = curr->next; //Forward current cell;
	}
	return best_move->single_source_moves_list; //Return the list that's the optimal move
}
 
//Function to change board accordingly, recieves current board, player and a list of moves
static void ChangeBoard(Board board, Player player, SingleSourceMovesList *best_possible_move)
{
	SingleSourceMovesListCell *curr_cell = best_possible_move->head; //Creating a cell to go through the list

	int currCol, currRow, newCol, newRow, side, mark = checkRange(player); //Variables for readability

	swapInBoard(board, *(best_possible_move->head->position), *(best_possible_move->tail->position)); //Take the player and put him in the end position

	while (curr_cell && curr_cell->next) //Loop for captures
	{
		currRow = (int)(curr_cell->position->row - 'A');
		currCol = (int)(curr_cell->position->col - '0');
		newRow = (int)(curr_cell->next->position->row - 'A');
		newCol = (int)(curr_cell->next->position->col - '0');

		if (currCol - newCol < 0) //Determine the side we are going to
			side = 1;
		else
			side = -1;

		if (currCol - newCol == -2 || currCol - newCol == 2) //If the skip is 2 columns that means we are capturing, meaning we need to empty the position in the middle
			board[newRow - mark][newCol - side] = ' ';

		curr_cell = curr_cell->next;
	}	
}

static void FreeMultiList(MultipleSourcesMovesList *List_of_lists) //Free list of lists
{
	MultipleSourceMovesListCell *curr = List_of_lists->head, *next;

	while(curr)
	{
		next = curr->next; //Save next cell
		freeList(curr->single_source_moves_list); //Free the list in the list of lists
		free(curr); //Free current cell
		curr = next; //Forward cell
	}
	free(List_of_lists); //Free the list of lists 
}

static void freeList(SingleSourceMovesList *list) //Same as above but with list and position
{
	SingleSourceMovesListCell *curr = list->head, *next;

	while(curr)
	{
		next = curr->next;
		free(curr->position);
		free(curr);
		curr = next;
	}
	free(list);
}
