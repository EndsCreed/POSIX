#include <stdio.h>

int main() {
    int i = 0; int sum = 0;
    while (i < 15) {
        sum += i;
        i++;
    }
    printf("The sum of the first 15 natural numbers is: %i", sum);
    return 0;
}