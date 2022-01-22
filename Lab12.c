//
// Created by Jonathan Mitchell on 1/21/22.
//

#include <stdio.h>
#include <ctype.h>

//This is a reusable function I will continue to use.
void cleanBuffer() { //So this is my solution to scanf leaving invalid inputs in the input buffer.
    int n;
    while((n = getchar()) != EOF && n != '\n' ); //Keeps clearing the next char from the buffer as long as there isn't a newline character and isn't the end of the file.
}

int main(int argc, char **argv) {
    char c;
    int n;
    float f;

    printf("Enter a character: "); //Getting a char from input.
    scanf("%c", &c);
    cleanBuffer();
    printf("Enter a number: "); //Getting a integer from input.
    scanf("%i", &n);
    cleanBuffer();
    printf("Enter a real number: "); //Getting a float from input.
    scanf("%f", &f);
    cleanBuffer();

    printf("Chracter: %c\nNumber: %i\nDecimal: %.2f\n", c, n, f);

    return 0;
}
