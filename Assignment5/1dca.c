/*
 * file: 1dca.c
 * One-Dimensional Cellular Automaton
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
/* #include "memcheck.h" */

/* which version of the update function should be used */
#define POINTER_VERSION     0

int main(int argc, char *arv[]);
int *randomGeneration(int num_cells);
void run(int num_gens, int num_cells, int *current_gen, int **generations);
int *update(int num_cells, int *current_gen);
int cellUpdate(int left, int center, int right);
void printGenerations(int num_gens, int num_cells, int **generations);
void memoryFail(void);

/*
 * main: run a one-dimensional cellular automaton
 * arguments: argc: the number of arguments
 *            argv: the arguments as strings
 *                  1: the number of cells per generation
 *                  2: the number of generations
 * return value: exit status
 */
int main(int argc, char *argv[])
{
    int num_cells, num_gens;
    int *initial_generation;
    int **generations;
    if (argc < 3)
    {
        printf("usage: 1dca <number of cells> <number of generations>\n");
        exit(EXIT_FAILURE);
    }
    num_cells = atoi(argv[1]);
    num_gens = atoi(argv[2]);
    initial_generation = randomGeneration(num_cells);
    generations = malloc(num_gens * sizeof(int *));
    if (generations == NULL) memoryFail();
    generations[0] = initial_generation;
    run(num_gens - 1, num_cells, initial_generation, generations + 1);
    printGenerations(num_gens, num_cells, generations);
    return EXIT_SUCCESS;
    /* print_memory_leaks(); */
}

/*
 * randomGeneration: create a random generation to start off the simulation
 * arguments: num_cells: the number of cells in the generation
 * return value: an array of psuedo-random 0s and 1s
 */
int *randomGeneration(num_cells)
{
    int *rand_gen;
    int i;
    rand_gen = malloc(num_cells * sizeof(int));
    if (rand_gen == NULL) memoryFail();
    srand(time(NULL));
    for (i = 0; i < num_cells; i++)
    {
        rand_gen[i] = (int) (((double) rand() / (double) RAND_MAX) * 1.999);
    }
    return rand_gen;
}

/*
 * run: run a one-dimensional cellular automaton
 * arguments: num_gens: the total number of generations to run
 *                      (excluding the given one)
 *            num_cells: the number of cells per generation
 *            current_gen: the current generation
 *            generations
 *            generations: an array of all the generations to be filled
 *                         starting with the next open value
 *                         (does not include the current generation
 *                         if not previously added)
 * return value: none (operates on a pointer to an array)
 */
void run(int num_gens, int num_cells, int *current_gen, int **generations)
{
    /* base case: if there are no generations to run, return */
    if (num_gens == 0) return;
    /* otherwise */
    /* set the value of the pointer to generations
     * (the first empty value of it) to the new generation */
    *generations = update(num_cells, current_gen);
    run(num_gens - 1, num_cells, *generations, generations + 1);
}

#if !POINTER_VERSION
/*
 * update: update the given array using the update rule
 * arguments: num_cells: the number of cells in the generation
 *            current_gen: the current generation
 * return value: a new generation with the update rule applied
 */
int *update(int num_cells, int *current_gen)
{
    int *new_gen;
    int i, left, right;
    new_gen = malloc(num_cells * sizeof(int));
    if (new_gen == NULL) memoryFail();
    for (i = 0; i < num_cells; i++)
    {
        if (i > 0) left = current_gen[i-1];
        else left = 0;
        if (i < num_cells - 1) right = current_gen[i+1];
        else right = 0;
        new_gen[i] = cellUpdate(left, current_gen[i], right);
    }
    return new_gen;
}
#else
/* see above for description */
int *update(int num_cells, int *current_gen)
{
    int *new_gen;
    /* left right and center and new array pointers */
    int *pl, *pc, *pr, *pn;
    int i;
    new_gen = malloc(num_cells * sizeof(int));
    pl = current_gen - 1; pc = current_gen; pr = current_gen + 1;
    pn = new_gen;
    if (new_gen == NULL) memoryFail();
    for (i = 0; i < num_cells; i++)
    {
        /* edge cases where there is no cell to one side */
        if (i == 0) *pn = cellUpdate(0, *pc, *pr);
        else if (i == num_cells - 1) *pn = cellUpdate(*pl, *pc, 0);
        /* otherwise */
        else *pn = cellUpdate(*pl, *pc, *pr);
        pl++; pc++; pr++; pn++;
    }
    return new_gen;
}
#endif

/*
 * updateCell: give the value for the cell when updated
 * arguments: left: the value to the left of the current cell
 *            center: the value of the current cell
 *            right: the value to the right of the current cell
 * return value: the new value of the cell
 */
int cellUpdate(int left, int center, int right)
{
    if (left + right == 1 && center == 0) return 1;
    else return 0;
}

/*
 * printGenerations: print an array of generations
 * arguments: num_gens: the number of generations in the array
 *            num_cells: the number of cells in each generation
 *            generations: the array of generations
 * return value: none
 */
void printGenerations(int num_gens, int num_cells, int **generations)
{
    int i, j;
    for (i = 0; i < num_gens; i++)
    {
        for (j = 0; j < num_cells; j++)
        {
            printf("%d", generations[i][j]);
        }
        printf("\n");
    }
}

/*
 * memoryFail: exit printing a notice that
 *             the machine was unable to allocate memory
 * arguments: none
 * return value: none
 */
void memoryFail()
{
    fprintf(stderr, "Unable to allocate memory; exiting");
    exit(EXIT_FAILURE);
}
