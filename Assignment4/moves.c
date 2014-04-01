/* FILE: moves.c */

/* for printing error message */
#include <stdio.h>
/* for standard library functions and definitions */
#include <stdlib.h>
/* for square root (sqrt) */
#include <math.h>
/* for copying memory (memcpy) */
#include <string.h>

#include "moves.h"

/* the maximum number of moves that can be made from a single hole */
#define MAX_MOVES   6

/* common definitions of TRUE and FALSE */
#define TRUE        1
#define FALSE       0

/* TODO, change layer number to start at 0 */

/* prototypes */
/* struct move_array allMoves(int triangle_layers); */
int numMoves(int side_length);
void memoryFail(void);
int triangularNumber(int side_length);
/* struct move_array movesAt(int start_pos, int num_layers); */
void addMove(int num_moves, int moves[][MOVE_LENGTH],
             int start_pos, int jump_pos, int end_pos);
struct move_array combine(int num_moves1, int move_array1[][MOVE_LENGTH],
                          int num_moves2, int move_array2[][MOVE_LENGTH]);
int layerNumber(int position);
void printMoveArray(struct move_array arr);

/*
 * TODO, fix counting issue, last layer only counts to n - 1
 * allMoves: return all moves that can be made in the triangular board
 *           (diagonal or horizontal jumping over one space to end
 *           in another valid position)
 * arguments: triangle_layers: the number of layers (and base length)
 *                             in the equilateral triangle
 * return value: an array of all valid moves
 *               (arrays of length three, {START, JUMP, END})
 */
struct move_array allMoves(int triangle_layers)
{
    /* loop index */
    int i;
    /* the current number of moves */
    int num_moves = 0;
    /* array of moves (to be allocated memory) */
    int (*moves)[MOVE_LENGTH];
    /*
     * contains returned pointers to
     * the number of new moves and the new moves
     */
    struct move_array return_array;
    /* the number of new moves */
    int num_new_moves;
    /* the array of new moves */
    int (*new_moves)[MOVE_LENGTH];
    /*
     * allocate the appropriate amount of memory for the moves
     */
    moves = malloc(sizeof(moves) * (numMoves(triangle_layers)));
    /*
     * if malloc was unable to allocate memory,
     * print an error message explaining and exit unsuccessfully
     */
    if (moves == NULL) memoryFail();
    for (i = 0; i < triangularNumber(triangle_layers); i++)
    {
        return_array = movesAt(i, triangle_layers);
        num_new_moves = return_array.length;
        new_moves = return_array.values;
        moves = combine(num_moves, moves, num_new_moves, new_moves).values;
        num_moves += num_new_moves;
    }
    return_array.length = num_moves;
    return_array.values = moves;
    return return_array;
}

/*
 * numMoves: return the number of possible moves in the triangle
 * arguments: side_length: the side length of the equilateral triangle
 * return value: the number of possible moves in the triangle
 */
int numMoves(int side_length)
{
    /*
     * if the side length is less than or equal to two,
     * there will be no sequences of three linear holes,
     * and thus no possible moves
     */
    if (side_length <= 2)
    {
        return 0;
    } else {
        /*
         * for each row added, there are an additional:
         *  2 * (side_length - 3) horizontal moves
         *  4 * (side_length - 3) diagonal moves
         */
        return 6 * (side_length - 3) + numMoves(side_length - 1);
    }
}

/*
 * memoryFail: print an error message stating that
 *             the system failed to allocate memory and unsuccessfully exit
 * arguments: none
 * return value: none
 */
void memoryFail(void)
{
    fprintf(stderr, "failed to allocate memory");
    exit(EXIT_FAILURE);
}

/*
 * triangularNumber: the nth (where n is the side_length) triangular number
 *                   (last index + 1)
 * arguments: side_length: the nth triangular number and the side length
 *                         of the equilateral triangle
 * return value: the nth triangular number
 *               (decremented by one to compensate
 *               for the triangle starting at 0)
 */
int triangularNumber(int side_length)
{
    return ((side_length * (side_length + 1)) / 2) - 1;
}

/*
 * movesAt: return an array of all possible moves in the category
 * arguments: start_pos: the starting point for any moves
 *            num_layers: the number of layers in the triangle
 * return value: an array of all the possible moves
 */
struct move_array movesAt(int start_pos, int num_layers)
{
    /* the layer number of the starting position */
    int layer_number;
    /* the distance from the left of the triangle (0..layer_number - 1) */
    int distance_left;
    /* the distance from the right of the triangle (0..layer_number - 1) */
    int distance_right;
    /* the array of moves for the starting position */
    int moves[MAX_MOVES][MOVE_LENGTH];
    /* the position that the current move jumps over */
    int jump_pos;
    /* the end position of the current move */
    int end_pos;
    /* the number of moves added to the array */
    int num_moves = 0;
    struct move_array return_array;
    int i, j;
    for (i = 0; i < MAX_MOVES; i++)
    {
        for (j = 0; j < MOVE_LENGTH; j++)
        {
            moves[i][j] += 0;
        }
    }
    layer_number = layerNumber(start_pos);
    distance_left = start_pos - triangularNumber(layer_number - 1) - 1;
    distance_right = layer_number - distance_left - 1;
    /*
    printf("layer number: %d\n", layer_number);
    printf("distance left: %d\n", distance_left);
    printf("distance right: %d\n", distance_right);
    */
    /* if there is room for a move to the left (diagonal or horizontal) */
    if (distance_left >= 2)
    {
        /* if true, there is necessarily room for a horizontal move */
        jump_pos = start_pos - 1;
        end_pos = start_pos - 2;
        addMove(num_moves, moves, start_pos, jump_pos, end_pos);
        num_moves++;
        /* if there is room for a move upward */
        if (layer_number >= 2)

        {
            jump_pos = start_pos - layer_number;
            end_pos = start_pos - (2 * layer_number - 1);
            addMove(num_moves, moves, start_pos, jump_pos, end_pos);
            num_moves++;
        }
    }
    /* if there is room for a move to the right (diagonal or horizontal) */
    if (distance_right >= 2)
    {
        /* if true, there is necessarily room for a horizontal move */
        jump_pos = start_pos + 1;
        end_pos = start_pos + 2;
        addMove(num_moves, moves, start_pos, jump_pos, end_pos);
        num_moves++;
        /* if there is room for a move upward */
        if (layer_number >= 2)
        {
            jump_pos = start_pos - (layer_number - 1);
            end_pos = start_pos - (2 * layer_number - 3);
            addMove(num_moves, moves, start_pos, jump_pos, end_pos);
            num_moves++;
        }
    }
    /* if there is room for a move downward */
    if (layer_number <= num_layers - 2)
    {
        /* if there is room for a move down to the right (SE) */
        jump_pos = start_pos + (layer_number + 1);
        end_pos = start_pos + (2 * layer_number + 3);
        addMove(num_moves, moves, start_pos, jump_pos, end_pos);
        num_moves++;
        /* if there is room for a move down to the left (SW) */
        jump_pos = start_pos + layer_number;
        end_pos = start_pos + (2 * layer_number + 1);
        addMove(num_moves, moves, start_pos, jump_pos, end_pos);
        num_moves++;
    }
    return_array.length = num_moves;
    return_array.values = moves;
    /* return return_array; */
    return return_array;
}

/*
 * addMove: add a move to the current array of moves
 * arguments: num_moves: the number of moves currently in the array
 *            moves: the current array of moves
 *            start_pos: the starting position in the triangle
 *            jump_pos: the position to be jumped over in the move
 *            end_pos: the ending position of the move
 * return value: none (operates on the reference to the array)
 * CAUTION: the array is assumed to have enough room to add another value
 */
void addMove(int num_moves, int moves[][MOVE_LENGTH],
             int start_pos, int jump_pos, int end_pos)
{
    moves[num_moves][START] = start_pos;
    moves[num_moves][JUMP] = jump_pos;
    moves[num_moves][END] = end_pos;
}

/*
 * combine: create a new array that is a
 *          combination of the two given move arrays
 * arguments: num_moves1: the number of values in the first array
 *            move_array1: the first array of moves
 *            num_moves2: the number of values in the second array
 *            move_array2: the second array of moves
 * return value: the new (combined) array
 */
struct move_array combine(int num_moves1, int move_array1[][MOVE_LENGTH],
                          int num_moves2, int move_array2[][MOVE_LENGTH])
{
    struct move_array return_array;
    int (*new_array)[MOVE_LENGTH];
    /* realloc would destroy the argument array */
    new_array = malloc(sizeof(int) * MOVE_LENGTH
                       * (num_moves1 + num_moves2));
    if (new_array == NULL) memoryFail();
    /*
     * copy the contents of the first array to the new array,
     * starting at the first value (the pointer for the array)
     */
    memcpy(new_array, move_array1, sizeof(int) * MOVE_LENGTH * num_moves1);
    /*
     * copy the contents of the second array to the new array,
     * starting at the end of the first array
     */
    memcpy(new_array + sizeof(int) * MOVE_LENGTH * num_moves1,
           move_array2, sizeof(int) * MOVE_LENGTH * num_moves2);
    return_array.length = num_moves1 + num_moves2;
    return_array.values = new_array;
    return return_array;
}

/*
 * TODO, find a better algorithm
 * layerNumber: the number of the triangle layer that the given position is on
 * arguments: position: the hole number (0...) to know the layer of
 * return value: the layer number (1...)
 */
int layerNumber(int position)
{
    /*
     * n * (n + 1) / 2 = t
     * n^2 + n - 2t = 0
     * n = (-1 +/- sqrt(1 - 4 * - 2t)) / 2
     * (- will produce a - number)
     * n = (-1 + sqrt(1 + 8t)) / 2
     * n = (-1 + sqrt(1 + 8(p + 1))) / 2
     * n = (-1 + sqrt(9 + 8p))) / 2
     */
    /* + .9 compensates for starting layers at n = 1 */
    return (int) (((-1 + sqrt(9 + 8 * position)) / 2) + 0.9);
}

void printMoveArray(struct move_array arr)
{
    int *move;
    int i;
    for (i = 0; i < arr.length; i++)
    {
        move = arr.values[i];
        printf("[%d, %d, %d]", move[START], move[JUMP], move[END]);
    }
    printf("\n");
}
