// Lab 25: Functions, Pointers, and Arrays.
// Jon Mitchell & Tom Arklie

#include <stdio.h>

void shift(int *a, int *b, int *c) {
    int temp = *c;
    // ^ Temp variable to set a to, since c will be overwritten
    *c = *b;
    *b = *a;
    *a = temp;
}

int main() {
    int a = 10, b = 25, c = 60;

    printf("a = %d\nb = %d\nc = %d\n\n", a, b, c);
    shift(&a, &b, &c);
    printf("a = %d\nb = %d\nc = %d", a, b, c);
}