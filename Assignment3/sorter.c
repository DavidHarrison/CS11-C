/* for standard input/output functions (printf, scanf) */
#include <stdio.h>
/* for exit code definitions */
#include <stdlib.h>
/* for string comparison (strcmp) */
#include <string.h>
/* for assert statement */
#include <assert.h>

/* the maximum allowed number of integer arguments */
#define MAX_INT_ARGS    32
/* standard definition of boolean values as ints */
#define TRUE            1
#define FALSE           0

/* function prototypes */
int main(int argc, char *argv[]);
void badArgExit(char *argv[]);
void minimumElementSort(int numc, int *numv);
void swap(int index1, int index2, int *numv);
void bubbleSort(int numc, int *numv);
int checkSort(int numc, int *numv);
void printValues(int numc, int *numv);

/*
 * main: read the arguments from argv and call the specified sorting function
 *       and print out the sorted list if quiet is not specified
 * arguments: argc: the number of arguments
 *            argv: the array of arguments
 * return value: an exit code for whether or not the program
 *               was successful (0/1)
 */
int main(int argc, char *argv[])
{
    /* the number of ints in the following list */
    int numc = 0;
    /* a list of ints of up to MAX_INT_ARGS in length */
    int numv[MAX_INT_ARGS];
    /* loop index */
    int i;
    /* set quiet to default to false (do not suppress output) */
    int quiet = FALSE;
    /* set default values for which sort to use */
    int minimum_element_sort = TRUE;
    int bubble_sort = FALSE;
    /*
     * loop through the arguments,
     * looking for options and integer arguments
     */
    for (i = 1; i < argc; i++) /* ignores invocation argument */
    {
        if (strcmp(argv[i], "-q") == 0)
        {
            quiet = TRUE;
        /* if the -b argument is found, switch to using bubble sort */
        } else if (strcmp(argv[i], "-b") == 0) {
            minimum_element_sort = FALSE;
            bubble_sort = TRUE;
        } else {
            numc++;
            /* 
             * if the number of number arguments has exceeded MAX_INT_ARGS,
             * exit with usage message
             */
            if (numc > MAX_INT_ARGS) badArgExit(argv);
            numv[numc - 1] = atoi(argv[i]);
        }
    }
    if (numc == 0) badArgExit(argv);
    /*
     * check which sort should be used, and call the appropriate function
     * since the array is passed a reference, it does not need to be
     * reassigned
     */
    if (minimum_element_sort)
    {
        minimumElementSort(numc, numv);
    } else if (bubble_sort) {
        bubbleSort(numc, numv);
    }
    /* check that the numbers are correctly sorted */
    assert(checkSort(numc, numv));
    /*
     * if the program has not been set to be quiet,
     * print out the sorted values one per line
     */
    if (!quiet) printValues(numc, numv);
    /* if there are no int arguments, exit with usage message */
    return EXIT_SUCCESS;
}

/*
 * badArgExit: exit with a failing status code (1), printing a usage message
 * arguments: none
 * return value: none
 */
void badArgExit(char *argv[])
{
    fprintf(stderr,
            "usage: %s [-b] [-q]"
            "number1 [number2 ... ] (maximum MAX_INT_ARGS numbers)",
            argv[0]);
    exit(EXIT_FAILURE);
}

/*
 * minimumElementSort: sort the given array
 *                     so that values are in ascending order
 *                     using a minimum element sort
 * arguments: numc: the number of values in numv
 *            numv: the array of ints
 * return value: none (operates on the reference to the array)
 */
void minimumElementSort(int numc, int *numv)
{
    /* outer loop index */
    int start;
    /* inner loop index */
    int i;
    /* the index of the minimum value encountered so far */
    int min_index;
    /*
     * after each run, one more element at the beginning of the array
     * will be sorted, skip these by incrementing start
     */
    for (start = 0; start < numc; start++)
    {
        /*
         * set the minimum value encountered to be the first value
         * in the non-sorted section of the array (that starting at start)
         */
        min_index = start;
        /*
         * for each of the subsequent index, check
         * if its value is smaller than that the current minimum index,
         * if so, set the current minimum index to the current index
         */
        for (i = start + 1; i < numc; i++)
        {
            if (numv[i] < numv[min_index])
            {
                min_index = i;
            }
        }
        /*
         * swap the values of the minimum value index
         * and the starting index
         */
        swap(start, min_index, numv);
    }
}

/*
 * swap: swap the values of the two given indices in the array
 * arguments: index1: the first index
 *            index2: the second index
 *            numv: the array
 * return value: none (operates on the passed reference)
 */
void swap(int index1, int index2, int *numv)
{
    /* temporary holder for value */
    int tmp;
    /* temporarily assign the value of index1 (to be overwritten) to tmp */
    tmp = numv[index1];
    /* assign the value of index1 to that of index2 */
    numv[index1] = numv[index2];
    /* assign the value of index2 to that of index1 (stored in tmp) */
    numv[index2] = tmp;
}

/*
 * bubbleSort: sort the given array
 *             so that values are in ascending order
 *             using a bubble sort
 * arguments: numc: the number of values in numv
 *            numv: the array of ints
 * return value: none (operates on the reference to the array)
 */
void bubbleSort(int numc, int *numv)
{
    /* the current first index that is sorted */
    int fsi;
    /* loop index */
    int i;
    for (fsi = numc; fsi > 0; fsi--)
    {
        for (i = 1; i < numc; i++)
        {
            if (numv[i] < numv[i - 1])
            {
                swap(i, i - 1, numv);
            }
        }
    }
}

/*
 * checkSort: check whether an array of ints is sorted
 *            (value in ascending order)
 * arguments: numc: the number of values in numv
 *            numv: the array
 * return value: whether or not the array is sorted
 *               (TRUE/FALSE respectively)
 */
int checkSort(int numc, int *numv)
{
    /* loop index */
    int i;
    /*
     * for each value in the array of ints which has a preceding value
     * (all except the first), check that it is bigger than its
     * preceding value, if not return false, otherwiser, return true
     */
    for (i = 1; i < numc; i++)
    {
        if (numv[i] < numv[i - 1]) return FALSE;
    }
    return TRUE;
}

/*
 * printValues: print the values from an array of ints, one per line
 * arguments: numc: the number of values in the array of ints
 *            numv: the array
 * return value: none
 */
void printValues(int numc, int *numv)
{
    /* loop index */
    int i;
    for (i = 0; i < numc; i++)
    {
        printf("%d\n", numv[i]);
    }
}
