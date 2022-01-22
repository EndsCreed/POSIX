// Lab 1 pt 4
// Created by Jonathan Mitchell on 1/22/22
// 20160512

#include <stdio.h>

int main(int argc, char **argv) {
    int grade = 0, i;
    struct { //Will be used to search the int values and strings in an array index.
        int value;
        const char *string;
    }
            ltrGrd[] = {
            {0, "F"},
            {50, "D"},
            {55, "C"},
            {65, "B"},
            {85, "A"}
    };
    printf("Enter a number grade: ");
    scanf("%i", &grade); //Now I don't need a string buffer for an int.
    printf("grade = %i", grade);
    if (grade < 0 || grade > 100) {
        printf("\nThe grade you have entered is invalid. Please re-run the program with a valid grade.");
    }
    else {
        for (i = 0; i < 4 && grade >= ltrGrd[i + 1].value; i++); //This for loop is explained at the end of the file.
        printf("\nGrade is: %s", ltrGrd[i].string); //After the for loop finishes, this prints the string at the current index.
    }
    return 0;
}

/** THE FOR LOOP HAS NO BODY ON PURPOSE!
     * THe purpose is to iterate through the list and compare the value entered for "grade" to a value in
     * an index in the array. This loop checks to see if the value of "grade" is greater than or equal to
     * the value in the next index. If it is (and the value of i is still within indexes 0 - 4) then it will
     * loop again and compare "grade" to the next value.
     *
     * Example: grade = 65; i = 0
     * Is 65 (value of grade) greater than or equal to the value at index 1 (i+1)? (i = 0, value = 50)
     * YES, increment i and compare again.
     * Is 65 greater than or equal to the value at index 2 (i+1)? (i = 1, value = 55)
     * YES, increment i and compare again.
     * Is 65 grater than or equal to the value at index 3 (i+1)? (i = 2, value = 65)
     * YES, increment i and compare again.
     * Is 65 greater or equal to the value at index 4 (i+1) (i = 3, value = 85)
     * NO, Do not increment i and break out of the for loop.
     * the grade is 65 or B. Since B is at index 3 and i = 3, that will be what is printed.
*/