//
// Created by Jonathan Mitchell on 1/22/22
// 20160512

#include <stdio.h>

//This is a reusable function I will continue to use.
void cleanBuffer() { //So this is my solution to scanF leaving invalid inputs in the input buffer.
    int n;
    while((n = getchar()) != EOF && n != '\n' ); //Keeps clearing the next char from the buffer as long as there isn't a newline character and isn't the end of the file.
}

int main(int argc, char **argv) {
    int target, current, ans;

    printf("How many loops do you wish to alternate?: ");
    scanf("%i", &target);
    cleanBuffer();

    for (int count = 1; count <= target; count++) {
        if (count % 2 == 0) { // If count is even we subtract as it's on an even number of loops.
            ans -= count;
            printf(" - %i", count);
        }
        else { // Otherwise we add.
            ans += count;
            if (count == 1) // Just to make the print a little nicer.
                printf("%i", count);
            else
                printf(" + %i", count);
        }
    }
    printf(" = %i\n", ans);

    return 0;
}