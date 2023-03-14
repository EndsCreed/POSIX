// Lab 22: Functions, Pointers, and Arrays.
// Jon Mitchell & Tom Arklie

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isPrime(int num) {
    if (num < 4)
        return num == 2 || num == 3;
    // ^ If the number is less than 4 then it is either negative, 0, 2, or 3.
    // The only two cases under 4 that are prime numbers are if num is 2 or 3.
    // 1 is not prime, 0 is not prime, nor is any negative number.
    // therefore, if num is less than 4 and is not 2 or 3, return false.
    int i = 2;
    while ((i * i) <= num) {
        if (num % i == 0)
            return 0;
        i++;
    }
    // To check for primes, we only need to check withing the root of the possible prime.
    // By keeping the while loop within without the sqrt of the num, we reduce the
    // time complexity to O(sqrt(num)). Within the loop we check if num is evenly
    // divisible by the current loop. If it is, it isn't prime. We start at 2 to exclude 1
    // and since we are only iterating to a maximum of the root of num, that excludes
    // num as a possible factor as well. Therefore, we now check if ANY number within
    // the range is a factor of num. If none are a factor, the number is prime, we exit
    // the loop and return true.
    return 1;
}

int isDigit(char *in) { // Verify the input is a digit.
    char *p = malloc(sizeof(char) * strlen(in));
    // ^ String to hold the result of atoi for comparison later.
    // Initialized to the char count of the input char[]
    // for optimal space complexity.
    int n = atoi(in);
    // ^ Attempting to make an int from the input
    sprintf(p, "%d", n);
    // ^ Taking the value of n gotten from atoi and writing it to
    if (in[strlen(in) - 1] == '\n')
        in[strlen(in) - 1] = '\0';
    return strcmp(in, p) == 0;
}

int main() {
    char buff[40];
    int num;

    printf("Enter any positive integer: ");
    fgets(buff, sizeof(buff), stdin);
    if (!isDigit(buff))
        return -1;
    // If it isn't a digit, return -1. We cannot test a non-digit for primality
    num = atoi(buff);
    return isPrime(num) ? printf("%d is prime", num) : printf("%d is not prime", num);
    // if isPrime is true, print that it is prime; else, print it is not.
}