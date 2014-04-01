/*
 * CS 11, C track, lab 8
 *
 * FILE: bci.c
 *       Implementation of the bytecode interpreter.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bci.h"


/* Define the virtual machine. */
vm_type vm;


/* Initialize the virtual machine. */
void init_vm(void)
{
    int i;

    /*
     * Initialize the stack.  It grows to the right i.e.
     * to higher memory.
     */

    vm.sp = 0;

    for (i = 0; i < STACK_SIZE; i++)
    {
        vm.stack[i] = 0;
    }

    /*
     * Initialize the registers to all zeroes.
     */

    for (i = 0; i < NREGS; i++)
    {
        vm.reg[i] = 0;
    }

    /*
     * Initialize the instruction buffer to all zeroes.
     */

    for (i = 0; i < MAX_INSTS; i++)
    {
        vm.inst[i] = 0;
    }

    vm.ip = 0;
}


/*
 * Helper function to read in integer values which take up varying
 * numbers of bytes from the instruction array 'vm.inst'.
 *
 * NOTES:
 * 1) This function moves 'vm.ip' past the integer's location
 *    in memory.
 * 2) This function assumes that integers take up 4 bytes and are
 *    arranged in a little-endian order (low-order bytes at the
 *    beginning).  This should hold for any pentium-based microprocessor.
 * 3) This function only works for n = 1, 2, or 4 bytes.
 *
 */

int read_n_byte_integer(int n)
{
    int i;
    unsigned char *val_ptr;
    /* unsigned char *init_val_ptr; */
    int val = 0;

    /* This only works for 1, 2, or 4 byte integers. */
    assert((n == 1) || (n == 2) || (n == 4));

    val_ptr = (unsigned char *)(&val);
    /* init_val_ptr = val_ptr; */

    for (i = 0; i < n; i++)
    {
        *val_ptr = vm.inst[vm.ip];
        val_ptr++;
        vm.ip++;
    }

    return val;
}


/*
 * Machine operations.
 */

void do_push(int n)
{
    /* if there is no room left on the stack */
    if (vm.sp >= STACK_SIZE - 1)
    {
        fprintf(stderr, "stack overflow on PUSH %d, exiting\n", n);
        exit(EXIT_FAILURE);
    }
    /* otherwise */
    /* add the value at the TOS (index of stack pointer) */
    vm.stack[vm.sp] = n;
    /* increment the stack pointer */
    vm.sp++;
}

void do_pop(void)
{
    /* if there is nothing on the stack */
    if (vm.sp <= 0)
    {
        fprintf(stderr, "failed to pop from an empty stack, exiting\n");
        exit(EXIT_FAILURE);
    }
    /* otherwise */
    /* decrement the stack pointer,
     * opening the memory at the current TOS to be overwritten */
    vm.sp--;
}

void do_load(int n)
{
    /* check to see if the registry index is invalid */
    check_registry_index(n);
    /* otherwise */
    /* push the value at the registry to the stack */
    do_push(vm.reg[n]);
}

void do_store(int n)
{
    /* check to see if the registry index is invalid */
    check_registry_index(n);
    /* otherwise */
    /* set the value of registry to be the value at TOS
     * (index of stack pointer) */
    vm.reg[n] = vm.stack[vm.sp - 1];
    /* pop the value from the stack */
    do_pop();
}

void do_jmp(int n)
{
    /* if the instruction index is invalid, exit */
    check_instruction_index(n);
    /* otherwise */
    /* set the instruction pointer to the new position */
    vm.ip = n;
}

void do_jz(int n)
{
    /* if the instruction index is invalid, exit */
    check_instruction_index(n);
    /* if the stack is empty, exit */
    check_stack_size(1);
    /* otherwise */
    /* if the TOS is zero, set the instruction pointer to the new position */
    if (vm.stack[vm.sp - 1] == 0)
    {
        vm.ip = n;
    }
    /* otherwise do nothing */
}

void do_jnz(int n)
{
    /* if the instruction index is invalid, exit */
    check_instruction_index(n);
    /* if the stack is empty, exit */
    check_stack_size(1);
    /* otherwise */
    /* if the TOS is not zero,
     * set the instruction pointer to the new position */
    if (vm.stack[vm.sp - 1] != 0)
    {
        vm.ip = n;
    }
    /* otherwise do nothing */
}

void do_add(void)
{
    /* if the stack does not have two values in it, exit */
    check_stack_size(2);
    /* otherwise */
    /* add S2 (TOS - 1) and S1 (TOS) */
    vm.stack[vm.sp - 2] = vm.stack[vm.sp - 2] + vm.stack[vm.sp - 1];
    /* pop the last (non-overwritten) value (TOS) */
    do_pop();
}

void do_sub(void)
{
    /* if the stack does not have two values in it, exit */
    check_stack_size(2);
    /* otherwise */
    /* subtract S1 (TOS) from S2 (TOS - 1) */
    vm.stack[vm.sp - 2] = vm.stack[vm.sp - 2] - vm.stack[vm.sp - 1];
    /* pop the last (non-overwritten) value (TOS) */
    do_pop();
}

void do_mul(void)
{
    /* if the stack does not have two values in it, exit */
    check_stack_size(2);
    /* otherwise */
    /* multiply S2 (TOS - 1) and S1 (TOS) */
    vm.stack[vm.sp - 2] = vm.stack[vm.sp - 2] * vm.stack[vm.sp - 1];
    /* pop the last (non-overwritten) value (TOS) */
    do_pop();
}

void do_div(void)
{
    /* if the stack does not have two values in it, exit */
    check_stack_size(2);
    /* otherwise */
    /* divide S2 (TOS - 1) by S1 (TOS) */
    vm.stack[vm.sp - 2] = vm.stack[vm.sp - 2] / vm.stack[vm.sp - 2];
    /* pop the last (non-overwritten) value (TOS) */
    do_pop();
}

void do_print(void)
{
    /* check that the stack has a value as TOS */
    check_stack_size(1);
    /* print out the TOS followed by a newline */
    fprintf(stdout, "%d\n", vm.stack[vm.sp - 1]);
    do_pop();
}

/* check to see that the registry index is valid */
void check_registry_index(unsigned char n)
{
    /* if the registry index is invalid */
    if (n >= NREGS || n < 0)
    {
        fprintf(stderr, "invalid registry index %d, exiting\n", n);
        exit(EXIT_FAILURE);
    }
}

/* check to see that the instruction index is valid */
void check_instruction_index(unsigned short n)
{
    /* if the registry index is invalid */
    if (n >= MAX_INSTS || n < 0)
    {
        fprintf(stderr, "invalid instruction index %d, exiting\n", n);
        exit(EXIT_FAILURE);
    }
}

/* check to see if the stack is at least the specified length */
void check_stack_size(unsigned char min_length)
{
    /* if the stack is not big enough */
    if (vm.sp < min_length)
    {
        fprintf(stderr,
        "operation needs %d operands on the stack, not enough found, exiting\n",
        min_length);
        exit(EXIT_FAILURE);
    }
}




/*
 * Stored program execution.
 */

/* Load the stored program into the VM. */
void load_program(FILE *fp)
{
    int nread;
    unsigned char *inst = vm.inst;

    do
    {
        /*
         * Read a single byte at a time and load it into the
         * 'vm.insts' array.  'fread' returns the number of bytes read,
         * or 0 if EOF is hit.
         */

        nread = fread(inst, 1, 1, fp);
        inst++;
    }
    while (nread > 0);
}



/* Execute the stored program in the VM. */
void execute_program(void)
{
    int val;

    vm.ip = 0;
    vm.sp = 0;

    while (1)
    {
        /*
         * Read each instruction and select what to do based on the
         * instruction.  For each instruction you may also have to
         * read in some number of bytes as the arguments to the
         * instruction.
         */

        switch (vm.inst[vm.ip])
        {
        case NOP:
            /* Skip to the next instruction. */
            vm.ip++;
            break;

        case PUSH:
            vm.ip++;

            /* Read in the next 4 bytes. */
            val = read_n_byte_integer(4);
            do_push(val);
            break;

        case POP:
            vm.ip++;
            /* pop the top of the stack */
            do_pop();
            break;

        case LOAD:
            vm.ip++;

            /* Read in the next byte. */
            val = read_n_byte_integer(1);
            do_load(val);
            break;

        case STORE:
            vm.ip++;
            /* store the value from the next registry location to the stack */
            /* 1 byte for the registry, assuming max value of 16 (4 bits) */
            val = read_n_byte_integer(1);
            do_store(val);
            break;

        case JMP:
            vm.ip++;

            /* Read in the next two bytes. */
            val = read_n_byte_integer(2);
            do_jmp(val);
            break;

        case JZ:
            vm.ip++;
            /* perform the conditional jump */
            /* use a two byte integer assuming a maximum instruction index of
             * 65535 (16 bits/2 bytes) */
            val = read_n_byte_integer(2);
            do_jz(val);
            break;

        case JNZ:
            vm.ip++;
            /* perform the conditional jump */
            /* use a two byte integer assuming a maximum instruction index of
             * 65535 (16 bits/2 bytes) */
            val = read_n_byte_integer(1);
            do_jnz(val);
            break;

        case ADD:
            vm.ip++;
            /* add the top two values on the stack */
            do_add();
            break;

        case SUB:
            vm.ip++;
            /* subtract the top two values on the stack */
            do_sub();
            break;

        case MUL:
            vm.ip++;
            /* multiply the top two values on the stack */
            do_mul();
            break;

        case DIV:
            vm.ip++;
            /* divide the top two values on the stack */
            do_div();
            break;

        case PRINT:
            vm.ip++;
            /* print the top of the stack */
            do_print();
            break;

        case STOP:
            return;

        default:
            fprintf(stderr, "execute_program: invalid instruction: %x\n",
                    vm.inst[vm.ip]);
            fprintf(stderr, "\taborting program!\n");
            return;
        }
    }
}


/* Run the program given the file name in which it's stored. */
void run_program(char *filename)
{
    FILE *fp;

    /* Open the file containing the bytecode. */
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "bci.c: run_program: "
               "error opening file %s; aborting.\n", filename);
        exit(1);
    }

    /* Initialize the virtual machine. */
    init_vm();

    /* Read the bytecode into the instruction buffer. */
    load_program(fp);

    /* Execute the program. */
    execute_program();

    /* Clean up. */
    fclose(fp);
}
