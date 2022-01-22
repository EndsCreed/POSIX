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
    int num, sum = 0, i = 1;

    printf("Enter the number you want to check for perfection: ");
    scanf("%i", &num);
    cleanBuffer();

    while (i < num) { //Coulda just used a for loop...
        if (num % i == 0)
            sum += i; //Adding the factors on the fly.
        i++;
    }
    if (num == sum) //Checking if the number is the sum of it's factors excluding itself. If it is then it is perfect.
        printf("%i is a perfect number!\n", num);
    else
        printf("%i is not a perfect number.\n", num);
}