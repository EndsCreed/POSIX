#include <stdio.h>
#include <stdlib.h>

int main() {
    char buff[20]; int n; char c; float f; // Declaring all variables
    printf("Enter integer: ");
    fgets(buff, sizeof(buff), stdin);
    n = atoi(buff); // Converting buffer to an integer
    printf("Enter float: ");
    fgets(buff, sizeof(buff), stdin);
    f = atof(buff); // Converting buffer to a float
    printf("Enter character: ");
    fgets(buff, sizeof(buff), stdin);
    c = buff[0]; // Getting first index of buffer for the character entered

    printf("%c is a character\n %i is an integer\n %f is a real number", c, n, f);
  return 0;
}
