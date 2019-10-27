//***********************************************************************************************************************************
// Q2: Function recieves a tree of a certain player position and returnes the optimal line of gameplay with most captures from all  *
// possible gameplayes available in the tree, if there's more than one then pick randomly (Mine picks the first one it encounters)  *
//***********************************************************************************************************************************

#include "Header.h"

////////////////////////////////////////////////////////////////////static functions////////////////////////////////////////////////////////////////////////////////////////////
static void                       FindSingleSourceOptimalMoveRec (SingleSourceMovesTreeNode *node, unsigned short max_captures, BOOL *max_found, SingleSourceMovesList *lst);
static unsigned short             findMaxCaptures                (SingleSourceMovesTreeNode *node);
static void                       addIntoList                    (SingleSourceMovesTreeNode *node, SingleSourceMovesList *lst);
static SingleSourceMovesListCell *createCell                     (SingleSourceMovesTreeNode *node);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Main question function
SingleSourceMovesList *FindSingleSourceOptimalMove(SingleSourceMovesTree *moves_tree) //question 2
{
	SingleSourceMovesList *res = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList)); 
	checkMalloc(res); //Allocating memory and checking allocation
	res->head = res->tail = NULL; //Creating empty list

	unsigned short max_captures = findMaxCaptures(moves_tree->source); //Finding the maximum possible captures in the tree
	
	BOOL max_found = FALSE;

	if(moves_tree->source)
		FindSingleSourceOptimalMoveRec(moves_tree->source, max_captures, &max_found, res); //Sending to recursion with list by reference to create list recursively

	return res; //Returning the list created
}

//Function to find maximum captures in the tree
static unsigned short findMaxCaptures(SingleSourceMovesTreeNode *node)
{
	unsigned short left_max = 0, right_max = 0; //Initializing variables for max on both sides

	if (node->next_move[LEFT] == NULL && node->next_move[RIGHT] == NULL) //If the node is a leaf return it's number of captures
		return node->total_captures_so_far;

	else
	{
		if (node->next_move[LEFT]) //If left node exists go into recursion and input max left captures into variable
			left_max = findMaxCaptures(node->next_move[LEFT]);
		if (node->next_move[RIGHT]) //Same for right tree
			right_max = findMaxCaptures(node->next_move[RIGHT]);

		return MAX(left_max, right_max); //Return the maximum between right max captures and left max captures
	}
}

//Recursive function to create list, recieves a tree node, the maximum captures, bollean variable for if found max and a list
static void FindSingleSourceOptimalMoveRec(SingleSourceMovesTreeNode *node, unsigned short max_captures, BOOL *max_found, SingleSourceMovesList *lst)
{
	if (node->next_move[LEFT] == NULL && node->next_move[RIGHT] == NULL) //If the node is a leaf
	{
		if (node->total_captures_so_far == max_captures) //If the leaf has the max captures
		{
			addIntoList(node, lst); //Add the node to the list
			*max_found = TRUE; //Change the flag to true because we found one leaf that has the maximum possible captures, this way the first node found with max captures will be the one played
		}
	}

	else //If node is not a leaf
	{
		if (node->next_move[LEFT]) //If there's a left path, recursively check it
			FindSingleSourceOptimalMoveRec(node->next_move[LEFT], max_captures, max_found, lst);

		if (!(*max_found) && node->next_move[RIGHT]) //If we didn't find the mamimum captures leaf on left tree we go into the right tree
			FindSingleSourceOptimalMoveRec(node->next_move[RIGHT], max_captures, max_found, lst);

		if (*max_found) //If maximum captures flag changed to TRUE it means we found a leaf we need
			addIntoList(node, lst); //Add this node to the head of the list as part of the path to the leaf we found
	}

	return;
}

//Function to add a cell to the head of the list
static void addIntoList(SingleSourceMovesTreeNode *node, SingleSourceMovesList *lst)
{
	SingleSourceMovesListCell *cell = createCell(node); //Creating a list cell using the tree node

	if (!lst->head) //If list is empty input cell as both head and tail 
		lst->head = lst->tail = cell;
	
	else //If list is not empty, point cell to current head and swap head to the cell created
	{
		cell->next = lst->head;
		lst->head = cell;
	}
}

//Function creates a list cell from a tree node
static SingleSourceMovesListCell *createCell(SingleSourceMovesTreeNode *node)
{
	SingleSourceMovesListCell *cell = (SingleSourceMovesListCell*)malloc(sizeof(SingleSourceMovesListCell)); //Allocating memory for the cell
	checkMalloc(cell); //Checking memory allocation
	cell->captures = node->total_captures_so_far; //Inputting current captures into cell

	//Creating position of cell using position of node
	cell->position = (checkersPos*)malloc(sizeof(checkersPos));
	checkMalloc(cell->position);
	cell->position->col = node->pos->col;
	cell->position->row = node->pos->row;

	cell->next = NULL; //Next is NULL

	return cell;
}
