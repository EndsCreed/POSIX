// Lab 1 pt 3
// Created by Jonathan Mitchell on 1/22/22
// 20160512

#include <stdio.h>

int main(int argc, char **argv) {
    int i = 1;
    int sum = 0;

    while (i <= 15) {
        sum += i;
        i++;
    }

    printf("Sum of first 15 natural numbers: %i\n", sum);

    return 0;
}