// Lab 21: Functions, Pointers, and Arrays.
// Jon Mitchell & Tom Arklie

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isEven(int n) {
    return n == 0 ? 1 : n % 2 == 0;
    // ^ return true or false depending on if n % 2 == 0 is true or false.
    // In this lab, 0 is considered to be an even number
}

int isDigit(char *in) { // Verify the input is a digit.
    char *p = malloc(sizeof(char) * strlen(in));
    // ^ String to hold the result of atoi for comparison later.
    // Initialized to the char count of the input char[]
    // for optimal space complexity.
    int n = atoi(in);
    // ^ Attempting to make an int from the input
    sprintf(p, "%d", n);
    // ^ Taking the value of n gotten from atoi and writing it to
    if (in[strlen(in) - 1] == '\n')
        in[strlen(in) - 1] = '\0';
    return strcmp(in, p) == 0;
}

int main() {
    char buff[40];
    int num;

    printf("Enter any positive integer: ");
    fgets(buff, sizeof(buff), stdin);
    if (!isDigit(buff))
        return -1;
    // ^ Checking to make sure the input was an integer
    num = atoi(buff);

    return isEven(num) ? printf("%d is even", num) : printf("%d is odd", num);
    // return "num is even" if it's even and "num is odd" if it's odd.
}