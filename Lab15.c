#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    void flushStdin(FILE*);
    char letter, *p = (char*) malloc(sizeof(char)*4);
    int grade;
    grade = -1;
    char *buff = (char*) malloc(sizeof(char)*4);
    printf("Enter grade from 0 to 100:");
    fgets(buff, sizeof(char) * 4, stdin);
    grade = atoi(buff);

    /*
     * In order to do the comparison below for checking if the input consisted only of digits, we need to handle
     * the extra newline character at the end of the buff array if the input is less than 3 chars.
     * Otherwise, the string comp will compare the newline character at the end of the buff to grade.
     * atoi doesn't carry the newline character so the function will see them as different even if they match.
     * we replace it with a nullterm character to signify the end of the array to C.
     */
    if (buff[strlen(buff) - 1] == '\n')
        buff[strlen(buff) - 1] = '\0';
    flushStdin(stdin); // Clear stdin

    /* The idea of the below function and below following conditional is to filter out non-digit inputs
     * Since atoi() will just put 0 in for letter inputs, assigning grade to this output would make it 0
     * which is legal in our letter conversion. Putting letters in IS NOT legal though.
     * In order to be able to tell if the result of atoi was from a failed parse of letters or a genuine 0 inputted
     * I take the value given by atoi and convert it to a string so that I can directly compare it to buff.
     * This way, if the user enters "abc", then grade will be '0'. When we convert grade to a string we will have
     * a string of "0" which we then compare to "abc". Since they are different, strcmp will not return 0, and we
     * have successfully texted if the input was a digit or not. If we really do input a digit, the string
     * representation of grade will be the same as buff, so we can confirm we have only digits.
     */
    sprintf(p, "%d", grade); // Print digit representation of grade to a string.
    if (strcmp(p, buff) != 0) { // Compare digit and literal strings
        return -3; // Input was found to be non-numerical
    }

    if (grade < 50) {
        if (grade < 0)
            return -1; // Input was found to be less than 0
        letter = 'F';
    }
    else if (grade < 55)
        letter = 'D';
    else if (grade < 65)
        letter = 'C';
    else if (grade < 85)
        letter = 'B';
    else {
        if (grade > 100)
            return -2; // Input was found to be greater than 100
        letter = 'A';
    }

    printf("Letter Grade: %c", letter);
    return 0;
}

void flushStdin(FILE *fp) { // Memory management to clear the input buffer for do while usage. Do while was removed as it conflicted with the lab instructions.
    fseek(fp, 0, SEEK_END);
}