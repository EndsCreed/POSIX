#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isDigit(char *in) { // Verify the input is a digit.
    int count = 0;
    int str_len = strlen(in);
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
    char *p = malloc(sizeof(char) * (str_len + 1));
    // ^ String to hold the result of atoi for comparison later.
    // Initialized to the char count of the input char[]
    // for optimal space complexity.
    int n = atoi(in);
    // ^ Attempting to make an int from the input
    sprintf(p, "%d", n);
    // ^ Taking the value of n gotten from atoi and writing it to
    if (in[str_len - 1] == '\n')
        in[str_len - 1] = '\0';
    return strcmp(in, p) == 0;
}

int main() {
    char buff[80];
    int n, *res, total = 0;
    printf("Enter number of alternating series: ");
    fgets(buff, sizeof(buff), stdin);

    if (!isDigit(buff)) {
        return -1;
    }
    n = atoi(buff);

    res = malloc(sizeof(int) * n);

    for (int i = 1; i <= n; i++) {
        if (i % 2 == 0) {
            res[i-1] = -i;
            total -= i;
        } else {
            res[i-1] = i;
            total += i;
        }
    }

    for (int i = 0; i < n; i++) {
        printf("%d ", res[i]);
    }
    printf("= %d\n", total);

    return 0;
}