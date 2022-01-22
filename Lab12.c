//
// Created by faarie on 1/21/22.
//

#include <stdio.h>

int main() {
    int i;
    char c;
    float f;

    scanf("Please enter an integer: %i\n", i);
    scanf("Please enter any character: %c\n", c);
    scanf("Please enter any decimal number: %f\n", f);

    printf("The Integer you entered was: %i\nThe character you entered was: %c\nThe decimal you entered was: %f", i, c, f);

    return 0;
}
