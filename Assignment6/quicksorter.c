/*
 * file: quicksorter.c
 * quicksort a list from stdin to stdout
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "linked_list.h"
/* #include "memcheck.h" */

#define FALSE   0
#define TRUE    1

int main(int argc, char *argv[]);
void usageExit(void);
node *quicksort(node *list);

/*
 * main: sort a list of integer arguments and print them
 * arguments: argc: the number of arguments
 *            argv: the arguments (int-representing strings,
 *                  with an option -q (quiet))
 * return value: exit status
 */
int main(int argc, char *argv[])
{
    node *nums, *sorted_nums;
    int i;
    /* set quiet to default to off */
    int quiet = 0;
    nums = NULL;
    /* parse the arguments */
    /* for each argument (skipping program name at 0)*/
    /* descend to keep the original ordering */
    for (i = argc - 1; i > 0; i--)
    {
        /* if -q is specified, set the program to be quiet */
        if (strcmp(argv[i], "-q") == 0)
        {
            quiet = TRUE;
        }
        /* otherwise, it is a number */
        else
        {
            /* add this number to the list */
            /* create an link a new node to the list,
             * set the list to be the new node (last node) */
            nums = create_node(atoi(argv[i]), nums);
        }
    }
    /* if no number arguments were given, exit with a usage message */
    if (nums == NULL) usageExit();
    /* sort the numbers */
    sorted_nums = quicksort(nums);
    /* free the unsorted number list */
    free_list(nums);
    /* check that the numbers are sorted */
    assert(is_sorted(sorted_nums));
    /* if the program has not be set to quiet,
     * print the sorted list of numbers */
    if (!quiet) print_list(sorted_nums);
    /* free the list of numbers (should be freed on exit anyway) */
    free_list(sorted_nums);
    /* exit successfully */
    return EXIT_SUCCESS;
}

/*
 * usageExit: print a usage message then exit with a failure
 */
void usageExit()
{
    printf("usage: quicksorter [-q] <numbers>\n");
    exit(EXIT_FAILURE);
}

/*
 * quicksort: use a quicksort algorithm to sort the given list 
 * arguments: list: a pointer to the first node in the list
 *                  (non-destructive)
 * return value: a pointer to the first node of the new list
 */
node *quicksort(node *list)
{
    /* declare the needed lists */
    node *pivot, *less, *more, *sorted_less, *sorted_more, *new;
    node *n;
    /* base case: list is already sorted */
    if (list == NULL || list->next == NULL)
    {
        return copy_list(list);
    }
    /* set pivot to point at the first node of list */
    pivot = list;
    /* set the lists to start with null values */
    less = NULL; more = NULL;
    /* for each node in the list (skip the pivot) */
    for (n = list->next; n != NULL; n = n->next)
    {
        /* if the node's value is less than or equal to that of the pivot,
         * add it to the less list (not including the pivot)*/
        if (n->data <= pivot->data)
        {
            /* add a new node with the new value to the list */
            less = create_node(n->data, less);
        }
        /* otherwise it is greater than the pivot,
         * so add it to the more list */
        else
        {
            /* add a new node with the new value to the list */
            more = create_node(n->data, more);
        }
    }
    /* sort the less list */
    sorted_less = quicksort(less);
    /* since the call is non-destructive, the old lists should be freed */
    free_list(less);
    /* sort the more list */
    sorted_more = quicksort(more);
    /* add the pivot value to the sorted more list
     * (where it will be first) */
    sorted_more = create_node(pivot->data, sorted_more);
    /* since the call is non-destructive, the old lists should be freed */
    free_list(more);
    /* append the to lists, and set new to point at the new list) */
    new = append_lists(sorted_less, sorted_more);
    /* free the holding partitions */
    free_list(sorted_less); free_list(sorted_more);
    /* return the new, sorted, list */
    return new;
}
