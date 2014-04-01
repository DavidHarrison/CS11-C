#include <stdio.h>
#include <stdlib.h>
#include "moves.h"

#define TEST_NUM    5

/* prototypes */
int testAllMoves(void);
int testMovesAt(int position, int num_layers);
void printMoveArray(struct move_array arr);

int main(void)
{
    int position;
    int num_layers;
    /*
    testAllMoves();
    */
    printf("Enter position\n");
    scanf("%d", &position);
    printf("Enter number of layers\n");
    scanf("%d", &num_layers);
    testMovesAt(position, num_layers);
    return 1;
}

int testAllMoves(void)
{
    struct move_array return_array;
    return_array = allMoves(TEST_NUM);
    printf("Moves:\n");
    printMoveArray(return_array);
    return 1;
}

int testMovesAt(int position, int num_layers)
{
    struct move_array return_array, arr2;
    return_array = movesAt(position, num_layers);
    arr2 = movesAt(position, num_layers);
    printf("Moves at %d:\n", position);
    printMoveArray(return_array);
    printMoveArray(arr2);
    return 1;
}

/*
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
*/
