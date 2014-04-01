#include <stdlib.h>
#include <stdio.h>
#include "triangle_routines.h"

#define FALSE   0
#define TRUE    1

#define BOARD_SIZE  15

/* the number of all possible moves on the board */
#define NUM_MOVES       36

/* the length of a move array */
#define MOVE_LENGTH     3
/* the indices of the move components */
#define FROM            0
#define JUMP            1
#define TO              2

/* maximum number of moves at one point (L,R,NE,SE,SW,NW) */
#define MAX_MOVES_AT_POINT  6

int all_moves[NUM_MOVES][MOVE_LENGTH] =
{
    { 0,  1,  3},
    { 3,  1,  0},
    { 1,  3,  6},
    { 6,  3,  1},
    { 3,  6, 10},
    {10,  6,  3},
    { 2,  4,  7},
    { 7,  4,  2},
    { 4,  7, 11},
    {11,  7,  4},
    { 5,  8, 12},
    {12,  8,  5},
    { 0,  2,  5},
    { 5,  2,  0},
    { 2,  5,  9},
    { 9,  5,  2},
    { 5,  9, 14},
    {14,  9,  5},
    { 1,  4,  8},
    { 8,  4,  1},
    { 4,  8, 13},
    {13,  8,  4},
    { 3,  7, 12},
    {12,  7,  3},
    { 3,  4,  5},
    { 5,  4,  3},
    { 6,  7,  8},
    { 8,  7,  6},
    { 7,  8,  9},
    { 9,  8,  7},
    {10, 11, 12},
    {12, 11, 10},
    {11, 12, 13},
    {13, 12, 11},
    {12, 13, 14},
    {14, 13, 12}
};

struct arr
{
    int size;
    int *values[MAX_MOVES_AT_POINT];
};

int main(void);
int solve(int *board);
int pegsOnBoard(int *board);
struct arr movesAt(int position, int *board);
int valid(int *move, int *board);
int makeMove(int *move, int *board);
int unmakeMove(int *move, int *board);

/*
 * main: get a board from the user and solve, if possible, for it
 * arguments: none
 * return value: exit status (EXIT_SUCCESS, EXIT_FAILURE)
 */
int main()
{
    int board[BOARD_SIZE];
    int solved;
    triangle_input(board);
    solved = solve(board);
    if (!solved)
    {
        printf("No solution to the given board\n");
    }
    return EXIT_SUCCESS;
}

/*
 * solve: recursively solve for the given board
 * arguments: the board (an array of boolean-representing ints)
 * return value: a TRUE/FALSE on whether or not a solution was found
 */
int solve(int *board)
{
    int i, j;
    int solved;
    struct arr moves;
    /* base case: if there is only one peg on the board, it is solved */
    if (pegsOnBoard(board) == 1)
    {
        triangle_print(board);
        return TRUE;
    }
    /* otherwise */
    /* for each peg */
    for (i = 0; i < BOARD_SIZE; i++)
    {
        /* find the possible moves
         * (could be zero, including if the peg is empty) */
        moves = movesAt(i, board);
        /* for each move */
        for (j = 0; j < moves.size; j++)
        {
            /* make the move */
            makeMove(moves.values[j], board);
            /* check if the move can solve the board */
            solved = solve(board);
            /* undo the move */
            unmakeMove(moves.values[j], board);
            /* if the move solves the board
             * print the board
             * and return TRUE */
            if (solved)
            {
                triangle_print(board);
                return TRUE;
            }
        }
    }
    /* if there are not solution creating moves, return FALSE */
    return FALSE;
}

/*
 * pegsOnBoard: count the number of pegs on the board
 * arguments: board: an array of boolean-representing ints
 * return value: the number (int) of pegs on the board
 */
int pegsOnBoard(int *board)
{
    int i;
    int count = 0;
    for (i = 0; i < BOARD_SIZE; i++)
    {
        if (board[i]) count++;
    }
    return count;
}

/*
 * movesAt: find all of the valid moves at a position on the board
 * arguments: position: the int representing the position on the board
 *            board: an array of boolean-representing ints
 * return value: an array structure (struct arr)
 *               with size and values for the valid moves
 */
struct arr movesAt(int position, int *board)
{
    struct arr moves;
    int i;
    int *move;
    moves.size = 0;
    /* if the starting position is not filled,
     * return a blank array (struct arr) with size of 0 */
    if (!board[position]) return moves;
    /* otherwise */
    /* for each move in all possible moves */
    for (i = 0; i < NUM_MOVES; i++)
    {
        move = all_moves[i];
        /* if the move starts with the starting position,
         * and it is valid on the board */
        if (move[FROM] == position && valid(move, board))
        {
            /* add the move to the array */
            moves.values[moves.size] = move;
            /* and increment the count */
            moves.size++;
        }
    }
    /* return the list of all valid moves at the point */
    return moves;
}

/*
 * valid: check whether or not a move is valid for the current board
 * arguments: move: the move to check (array of length MOVE_LENGTH)
 *            board: an array of boolean-representing ints
 * return value: whether or not the move is valid (TRUE/FALSE)
 */
int valid(int *move, int *board)
{
    int from, jump, to;
    from = move[FROM];
    jump = move[JUMP];
    to = move[TO];
    /* check that the board is
     * filled at the from (starting) position
     * filled at the jump position
     * and open at the to (end) position
     * return the result */
    return (board[from] && board[jump] && !board[to]);
}

/*
 * move: make a move on the board
 * arguments: move: an array representing a move on the board
 *            board: an array of boolean-representing ints 
 * return value: whether or not the move was a success/possible
 */
int makeMove(int *move, int *board)
{
    int from, jump, to;
    /* if the move is not valid, return FALSE */
    if (!valid(move, board)) return FALSE;
    /* otherwise */
    from = move[FROM];
    jump = move[JUMP];
    to = move[TO];
    /* flip the open/occupied value for each position
     * (could be implemented as a loop) */
    board[from] = !board[from];
    board[jump] = !board[jump];
    board[to] = !board[to];
    return TRUE;
}

/*
 * unmove: reverse a move on the board
 * arguments: move: an array representing a move on the board
 *            board: an array of boolean-representing ints 
 * return value: whether or not the move was a success/possible
 */
int unmakeMove(int *move, int *board)
{
    int from, jump, to;
    from = move[FROM];
    jump = move[JUMP];
    to = move[TO];
    /* if it is not valid to reverse the move, return FALSE */
    if (!(!board[from] && !board[jump] && board[to])) return FALSE;
    /* otherwise */
    /* flip the open/occupied value for each position
     * (could be implemented as a loop) */
    board[from] = !board[from];
    board[jump] = !board[jump];
    board[to] = !board[to];
    return TRUE;
}
