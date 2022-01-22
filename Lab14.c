// Lab 1 pt 4
// Created by Jonathan Mitchell on 1/22/22
// 20160512

#include <stdio.h>

int main(int argc, char **argv) {
    int a = 6;
    int b = 19;

    int sum = a + b;
    int diff = a - b;
    int prod = a * b;
    float quot = (float) a / (float) b; //Casting to a float to take care of the decimals.
    int remain = a % b;

    printf("Sum: %i + %i = %i\n", a, b, sum);
    printf("Difference: %i - %i = %i\n", a, b, diff);
    printf("Product: %i x %i = %i\n", a, b, prod);
    printf("Quotient: %i / %i = %.2f\n", a, b, quot);
    printf("Remainder: %i %% %i = %i\n", a, b, remain);

    return 0;
}