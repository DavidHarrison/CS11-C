/*
 * CS 11, C Track, week 7
 *
 * FILE: main.c
 *
 *       Tests of the hash table implementation.
 *
 */

/*
 * Extra Credit Question:
 * As we mentioned above, you aren't supposed to change anything other
 * than the definitions in the template file. However, the resulting design
 * is not the cleanest possible design from the standpoint of managing memory.
 * If you were allowed to change something in the file main.c as well as in
 * the template file, how would you do this in order to create a cleaner
 * design? Why would this design be better than the existing design?
 * You can put your answer in comments in the main.c file. Don't change any
 * of the code in the main.c file, however; just say what you would do.
 * Also, leave a note in the comments at the top of the hash_table.c file
 * that you've modified main.c, so we know we should look at it.
 *
 * Response:
 * If I were to change the design of the main function, I would have
 * it take on the responsibility of freeing the string pointers. Having the
 * hash table manager free the strings then limits their use in the main
 * functions, whatever it may be doing. If this is something where it will
 * need to access the strings again after the hash table has been freed, it
 * forces (as is done in this main file) it to make a copy of each string
 * before passing it to the hash table. While this cost is not hugely
 * significant for this project, on a system with a very large data set and/or
 * limited resources, duplicating data like this can be wasteful.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "memcheck.h"

#define MAX_WORD_LENGTH 100


void usage(char *progname)
{
    fprintf(stderr, "usage: %s filename\n", progname);
}

void add_to_hash_table(hash_table *ht, char *key)
{
    int v = get_value(ht, key);
    set_value(ht, key, v + 1);
}


int main(int argc, char **argv)
{
    int   nwords;
    FILE *input_file;
    char  word[MAX_WORD_LENGTH];
    char  line[MAX_WORD_LENGTH];
    char *new_word;
    hash_table *ht;

    if (argc != 2)
    {
        usage(argv[0]);
        exit(1);
    }

    /* Make the hash table. */
    ht = create_hash_table();

    /*
     * Open the input file.  For simplicity, we specify that the
     * input file has to contain exactly one word per line.
     */
    input_file = fopen(argv[1], "r");

    if (input_file == NULL)  /* Open failed. */
    {
        fprintf(stderr, "Input file \"%s\" does not exist! "
                        "Terminating program.\n", argv[1]);
        return 1;
    }

    /* Add the words to the hash table until there are none left. */

    while (fgets(line, MAX_WORD_LENGTH, input_file) != NULL)
    {
        /* Clear the contents of 'word'. */
        word[0] = '\0';

        /* Convert the line to a word. */
        nwords = sscanf(line, "%s", word);

        if (nwords != 1)  /* Conversion failed, e.g. due to a blank line. */
        {
            continue;
        }
        else
        {
            /* Copy the word.  Add 1 for the zero byte at the end. */
            new_word = (char *)calloc(strlen(word) + 1, sizeof(char));

            if (new_word == NULL)
            {
                fprintf(stderr, "Error: memory allocation failed! "
                                "Terminating program.\n");
                return 1;
            }

            strcpy(new_word, word);

            /* Add it to the hash table. */
            add_to_hash_table(ht, new_word);
        }
    }

    /* Print out the hash table key/value pairs. */
    print_hash_table(ht);

    /* Clean up. */
    free_hash_table(ht);
    fclose(input_file);

    /* Check for memory leaks. */
    print_memory_leaks();

    return 0;
}
