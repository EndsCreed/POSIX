//
// Created by Jonathan Mitchell on 1/22/22
// 20160512

#include <stdio.h>

//This is a reusable function I will continue to use.
void cleanBuffer() { //So this is my solution to scanF leaving invalid inputs in the input buffer.
    int n;
    while((n = getchar()) != EOF && n != '\n' ); //Keeps clearing the next char from the buffer as long as there isn't a newline character and isn't the end of the file.
}

//Main
int main(int argc, char **argv) {
    int grade = -1; //Starting at -1 so that character inputs are also invalid.
    char *ltrGrd;
    printf("Enter a number grade: ");
    scanf("%i", &grade);
    cleanBuffer(); //Just trying to instill a good practise by cleaning my buffer. Might improve this function later.

    if (grade >= 0 && grade < 50)
        ltrGrd = "F";
    else if (grade >= 50 && grade < 55)
        ltrGrd = "D";
    else if (grade >= 55 && grade < 65)
        ltrGrd = "C";
    else if (grade >= 65 && grade < 85)
        ltrGrd = "B";
    else if (grade >= 85 && grade <= 100)
        ltrGrd = "A";
    else {
        printf("Invalid input. Please enter a number from 0 to 100 for the number grade!");
        return 1; //Returning to make sure the program exits when the input is invalid. Returning 1 for debugging purposes.
    }
    printf("A grade of %i is equal to a %s", grade, ltrGrd);

    return 0;
}