// Lab 24: Functions, Pointers, and Arrays.
// Jon Mitchell & Tom Arklie

#include <stdio.h>

int main() {
    int num = 5, *numptr = &num;
    char ltr = 'g', *ltrptr = &ltr;
    // ^ Declaring an int and a char and then declaring a pointer set to the address of the respective variables.

    printf("int: %d, pointer: %p, deref: %d\nchar: %c, pointer: %p, deref: %c\n\n", num, (void *) numptr, *numptr, ltr, (void *) ltrptr, *ltrptr);
    // ^ Prints the values of the variable, the address of the pointer, and the dereferenced value from said pointer for both the int and the char
    // The fields were the pointers are having their address printed are casted to void * as that's what %p is supposed to do.
    // Due to the implicit nature of %p, some machines may not print the address properly. Some cases it may not even print in Hex
    // As such, I explicitly casted to void * to ensure consistency on all machines.

    *numptr = 25;
    *ltrptr = 'z';
    // ^ Reassigning values to the int and the char variable through their pointer calls.

    printf("int: %d, pointer: %p, deref: %d\nchar: %c, pointer: %p, deref: %c", num, (void *) numptr, *numptr, ltr, (void *) ltrptr, *ltrptr);
    // ^ Another print out of information from the int, char, and their respective pointers. Note the memory address does not change.
}
