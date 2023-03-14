#include <stdio.h>
#include <stdlib.h>

int main() {
    int sum(const int*, const int*), diff(const int*, const int*), mult(const int*, const int*), remain(const int*, const int*);
    double divide(const int*, const int*);
    char buff[80]; int a, b;
    printf("Enter first integer: ");
    fgets(buff, sizeof(buff), stdin);
    a = atoi(buff);
    printf("Enter second integer: ");
    fgets(buff, sizeof(buff),stdin);
    b = atoi(buff);
    printf(
            "Sum: %i\n"
            "Difference: %i\n"
            "Product: %i\n"
            "Quotient: %f\n"
            "Remainder: %i\n",
            sum(&a, &b), diff(&a, &b), mult(&a, &b), divide(&a, &b), remain(&a, &b));

    return 0;
}

int sum(const int *a, const int *b) {
    return *a + *b;
}

int diff(const int *a, const int *b) {
    return *a - *b;
}

int mult(const int *a, const int *b) {
    return *a * *b;
}

double divide(const int *a, const int *b) {
    if (*b == 0) {
        printf("Division by 0 found. Quotient and Remainder will be -1\n");
        return -1;
    }
    return (double)*a / (double)*b;
}

int remain(const int *a, const int *b) {
    if (*b == 0) {
        return -1;
    }
    return *a % *b;
}