//*********************************************************************************************************************************************
// Karina Batmanishvili                                                                                                                       *
// Id: 321800898                                                                                                                              *
// Header file for all the c files, each question separated and has it's structs along with the functions that are general use in that c file *
//*********************************************************************************************************************************************

//Includes
#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

#include <stdio.h>
#include <string.h>

//Defines and typedefs
#define BOARD_SIZE 8

//Bolean function
typedef unsigned int BOOL;
#define FALSE      0
#define TRUE       1

//Defining the side we are in the array pointer
typedef unsigned int SIDE;
#define LEFT       0
#define RIGHT      1

#define STOP      -1

//Defining a short max function
#define MAX(a,b) ((a) > (b) ? (a) : (b))

//Struct for the checker position in characters
typedef struct _checkersPos
{
	char row; //Letters A-H
	char col; //Numbers 1-8
} checkersPos;

//Defining a matrix that is the board and it is eight on eight
typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];

//Defining the player in use TOP or BOTTOM - T or B
typedef unsigned char Player;

///////////////////////////////////////////////////////////////////////////////////QUESTION 1///////////////////////////////////////////////////////////////////////////////////////////////////////
//Struct for a single node in the tree includes an updated board, the current position in the board, total captures, array of pointers to the next nodes (both left and right)
typedef struct _SingleSourceMovesTreeNode {
	Board                               board;
	checkersPos                        *pos;
	unsigned short                      total_captures_so_far;
	struct _SingleSourceMovesTreeNode  *next_move[2];
}SingleSourceMovesTreeNode;

//Struct for the tree root
typedef struct _SingleSourceMovesTree {
	SingleSourceMovesTreeNode           *source;
}SingleSourceMovesTree;

SingleSourceMovesTree     *FindSingleSourceMoves (Board board, checkersPos *src); //Main question 1
void                       swapInBoard           (Board board, checkersPos curr, checkersPos next); //Swaps two places in the board
int                        checkRange            (Player player); //Function to let us know if player is Bottom or Top and returns the indicator to mark the placement and to use for row movement
void                       checkMalloc           (void *pointer);

///////////////////////////////////////////////////////////////////////////////////QUESTION 2///////////////////////////////////////////////////////////////////////////////////////////////////////
//Struct for a single cell in the list includes the current position in the board, total captures and pointer to the next cell
typedef struct _SingleSourceMovesListCell {
	checkersPos                       *position;
	unsigned short                     captures;
	struct _SingleSourceMovesListCell *next;
}SingleSourceMovesListCell;

//Struct for the list head and tail
typedef struct _SingleSourceMovesList {
	SingleSourceMovesListCell *head;
	SingleSourceMovesListCell *tail;
}SingleSourceMovesList;

//Main question 2 function
SingleSourceMovesList     *FindSingleSourceOptimalMove    (SingleSourceMovesTree *moves_tree); 

///////////////////////////////////////////////////////////////////////////////////QUESTION 3///////////////////////////////////////////////////////////////////////////////////////////////////////
//Struct for a single cell in the list includes all the possible moves available in the board for current player and pointer to the next cell
typedef struct _multipleSourceMovesListCell {
	SingleSourceMovesList               *single_source_moves_list;
	struct _multipleSourceMovesListCell *next;
}MultipleSourceMovesListCell;

//Struct for the list head and tail
typedef struct _multipleSourceMovesList {
	MultipleSourceMovesListCell *head;
	MultipleSourceMovesListCell *tail;
}MultipleSourcesMovesList;

//Functions for question 3
MultipleSourcesMovesList *FindAllPossiblePlayerMoves  (Board board, Player player);

///////////////////////////////////////////////////////////////////////////////////QUESTION 4///////////////////////////////////////////////////////////////////////////////////////////////////////
void Turn(Board board, Player player);

///////////////////////////////////////////////////////////////////////////////////QUESTION 5///////////////////////////////////////////////////////////////////////////////////////////////////////
void StoreBoard (Board board, char *filename);
void checkFile  (FILE *file);

///////////////////////////////////////////////////////////////////////////////////QUESTION 6///////////////////////////////////////////////////////////////////////////////////////////////////////
void LoadBoard  (char *filename, Board board);

///////////////////////////////////////////////////////////////////////////////////QUESTION 7///////////////////////////////////////////////////////////////////////////////////////////////////////
void PlayGame(Board board, Player starting_player);
void PrintBoard(Board board);
