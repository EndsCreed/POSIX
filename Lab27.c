// Lab 27: Functions, Pointers, and Arrays.
// Jon Mitchell & Tom Arklie

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isDigit(char *in) { // Verify the input is a digit.
    int count = 0;
    // ^ Will be responsible for counding leading zeros... should there be any.
    for (int i = 0; in[0] == '0' && i < strlen(in); i++) {
        // ^ First checks if index 0 of the entered number string is a 0, if so, start the removal.
        if (in[i] == '0') {
            count++;
            // ^ Each time we find a 0 at the start of the string, we'll increment 'count' as the count of leading zeros
        } else {
            // ^ As soon as we find a non-zero, we start removing them and then move on to the rest of the method.
            for (int j = count; j <= strlen(in); j++) {
                // ^ Start iterating at the first index with a non-zero. This is so we don't iterate more than we need to
                // Next we check if the current index is greater or equal to the string length.
                // The off-by-one error here is actually intentional as the first index after the array is a null termination
                // character. We'll want to move that too so that we can actually shorten our array.
                in[j-count] = in[j];
                // ^ Rather than walking down 1 index at a time, we can just jump down the entire count of leading zero's
                // effectively removing all of them at once.
            }
        }
    }
    char *p = malloc(sizeof(char) * (strlen(in) + 1));
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

int parseSpaceSeperated(char *in, int *nums) {
    int begin = 0, count = 0;
    // ^ We need a number array of 5, a begin index marker to mark the start of a number, and the count of numbers found
    char *p = malloc(sizeof(in));
    // ^ We need a buffer that can handle the substrings we'll make to parse the spaces in the input.

    for (int i = 0; i < strlen(in); i++) {
        // ^ As long as i is less than the string length of the input and we haven't yet found 5 numbers, loop.
        if (in[i] == '\0')
            return count;
        // ^ If we find a null term, then we end the loop with what we got. This is an assurance to stop seg faults.
        if (in[i] == ' ' || in[i] == '\n') {
            // If we find a space or newline, continue with the parsing.
            memcpy(p, &in[begin], i - begin);
            // ^ Copy from the last marked beginning point to current index
            // This is done by calculating the length of the current number by subtracting begin from the current index.
            p[(i)-begin] = '\0';
            // ^ Put a null term at the end of the found number.
            // Since we reuse the same buffer, we want to make sure we don't confuse atoi with possible chars from
            // previous numbers.
            if (!isDigit(p))
                return -1;
            // ^ If we find a non-digit, we exit the program with error code -1
            nums[count] = atoi(p);
            // ^ Add the new-found number to the number array. The index it is put in is determined by how many numbers
            // we have found so far.
            count++;
            begin = i + 1;
            // Increment count and set the new beginning point for the next number.
            if (in[i] == '\n' || count >= 5) {
                return count;
            }
            // If the found number was found by a newline, we are done. Break loop.
        }
    }
    return count;
    // ^ Just in-case size is 0
}

int sum(int const *array, int const *size) {
    int sum = 0;
    for (int i = 0; i < *size; i++) {
        sum += array[i];
    }
    // ^ Iterate through the array and add each value to sum as we find them.
    return sum;
}

int findLargest(int const *array, int const *size) {
    int largest = array[0];
    for (int i = 0; i < *size; i++) {
        if (array[i] > largest)
            largest = array[i];
    }
    // ^ As we find a new largest, set largest to that.
    return largest;
}

int findSmallest(int const *array, int const *size) {
    int smallest = array[0];
    for (int i = 0; i < *size; i++) {
        if (array[i] < smallest)
            smallest = array[i];
    }
    // ^ As we find a new smallest, set smallest to that.
    return smallest;
}

int main() {
    char buff[80];
    int array[5], count;
    printf("Enter 5 numbers seperated by spaces: ");
    fgets(buff, sizeof(buff), stdin);
    count = parseSpaceSeperated(buff, &array);
    // ^ Must be called before doing any functions on the array. This function initializes the array from your input.

    if (count == -1)
        return -1;
    // If count was set to -1, parseSpaceSeperated found a char in the numbers and aborted conversion
    for (int i = 0; i < count; i++)
        printf("%d ", array[i]);
    // Print the newly initialized int array up to the size returned by parseSpaceSeperated

    printf("\nSum: %d\nLargest: %d\nSmallest: %d\n", sum(&array, &count), findLargest(&array, &count), findSmallest(&array, &count));

    return 0;
}
