#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Explanation of logic:
 *
 * This is a bog standard priority queue which does not round robin.
 */

typedef struct {
    short RUNNING;
    int PID;
    int ARRIVAL_TIME;
    int REMAINING_TIME;
    int PRIORITY;
    int COMPLETION_TIME;
} Process;

Process initProcess(int PID, int AT, int RT, int PRI) { // PID, Arrival Time, Remaining Time, Priority
    Process p;
    p.RUNNING = 0;
    p.PID = PID;
    p.ARRIVAL_TIME = AT;
    p.REMAINING_TIME = RT;
    p.PRIORITY = PRI;
    return p;
}

Process *createDummyProcess () {
    Process p;
    p.RUNNING = -1;
    p.PID = -1;
    p.ARRIVAL_TIME = -1;
    p.REMAINING_TIME = -1;
    p.PRIORITY = -1;
    return &p;
}

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

int parseSpaceSeperated(char *in, int *nums) {
    int begin = 0, count = 0, str_len = strlen(in);
    // ^ We need a number array of 5, a begin index marker to mark the start of a number, and the count of numbers found
    char *p = malloc(sizeof(in));
    // ^ We need a buffer that can handle the substrings we'll make to parse the spaces in the input.

    for (int i = 0; i < str_len; i++) {
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

Process *findByPID(int *PID, Process *pList, int *TOTAL) {
    Process *p;
    for (int i = 0; i < *TOTAL; i++) {
        if (pList[i].PID == *PID)
            return &pList[i];
    }
    return createDummyProcess();
}

Process *getNextProcess(Process *pList, int *TOTAL, Process *lList, int *lCount, Process *mList, int *mCount, Process *hList, int *hCount, int *time) {
    Process *next, *p;
    int index = 0;
    while (index < *TOTAL) {
        if (pList[index].RUNNING == 0) {
            next = &pList[index];
            index = -1;
            break;
        }
        index++;
    }
    if (index != -1) {
        return createDummyProcess();
    }
    // ^ If there are no processes left,
    while (index < *hCount) {
        if (hList[index].RUNNING == -1) {
            index++;
            continue;
        }
        p = findByPID(&hList[index].PID, &pList, TOTAL);
        if (p-> ARRIVAL_TIME < next->ARRIVAL_TIME)
            next = p;
        break;
    }
    // ^ Check high priority queue for the lowest arrival time that isn't running
    index = 0;
    while (index < *mCount) {
        if (mList[index].RUNNING == -1) {
            index++;
            continue;
        }
        p = findByPID(&mList[index].PID, &pList, TOTAL);
        if (mList[index].ARRIVAL_TIME < p->ARRIVAL_TIME)
            next = p;
        break;
    }
    //
    index = 0;
    while (index < *lCount) {
        if (lList[index].RUNNING == -1) {
            index++;
            continue;
        }
        p = findByPID(&lList[index].PID, &pList, TOTAL);
        if (lList[index].ARRIVAL_TIME < p->ARRIVAL_TIME)
            next = p;
        break;
    }
    // ^ Scan through each list starting with high priority and only replace the next program
    // with a program who arrives first. Going from highest to lowest priority. That way, only programs with a
    // lesser arrival time will be able to run instead of high priority programs. This also gives
    // priority to programs with the same arrival time but higher priority by only replacing them
    // if the arrival time is LOWER.

    return next;
    // ^ Return the address of the process who isn't running but has the lowest arrival time and then highest
    // priority.
}

void sortProcesses(Process *pList, const int *TOTAL, int *foundList, int *found) {
    int next = 0, present = 0;
    while (*TOTAL > *found) {
    // ^ While we haven't found all processes and ordered them
        for (int i = 0; i < *TOTAL; i++) {
            // ^ While the current iteration is under the total count of processes and we haven't found every process
            if (pList[i].ARRIVAL_TIME < pList[next].ARRIVAL_TIME) {
                // ^ Check if the current processes arrival time is less than the last found "best".
                for (int z = 0; z < *found; z++) {
                    // ^ If it is considered better, loop through the current list of found processes
                    present = (pList[i].PID == pList[found[z]].PID) ? 1 : present;
                    // ^ On each found process, check if the PID of a prev found process matches the current new "best" process
                    // ^ If it does match, set "present" to 1, otherwise, keep it at what it was set to before. (0 if not found yet or 1 if it has been found)
                }
                next = (present) ? next : i;
                // ^ If the process was found in the found list,
            }
        }
        foundList[*found] = next;
        // ^ After we reach the end of the list of processes, add the found next best to the found list at the index of the total
        // number of found processes. Since array index's start at 0 but we count from 1, this should add the process to the
        // index before the total found count. Avoiding the off-by-one error.
        *found++;
        // ^ Increment the found count
    }
}

int main() {
    char buff[80];
    int buffatoi, TOTAL_PROCESSES;

    // GETTING NUMBER OF PROCESSES
    printf("Enter the number of process (Max 20): ");
    fgets(buff, sizeof(buff), stdin);

    if (!isDigit(buff))
        return -1;
    // ^ If inputted number of process is not a valid number, exit.

    buffatoi = atoi(buff);
    buff[0] = '\0'; // Empty Buffer
    TOTAL_PROCESSES = (buffatoi >= 0 && buffatoi <= 20) ? buffatoi : -1;
    if (TOTAL_PROCESSES == -1)
        return -2;
    // ^ If the number of processes entered is not from 0 to 20, exit.

    // SETTING EACH PROCESSES PROPERTIES
    Process processList[TOTAL_PROCESSES];
    int inNums[3], count, lowPri = 0, medPri = 0, hiPri = 0;

    for (int i = 0; i < TOTAL_PROCESSES; i++) {
        printf("Enter the Arrival Time, Burst Time, and Priority of seperated by spaces for Process %d", i);
        fgets(buff, sizeof(buff), stdin);
        count = parseSpaceSeperated(buff, &inNums);
        // ^ Set inputs into the Input Numbers array.

        if (count != 3)
            return -3;
        // ^ If count is not the right amount of inputs, exit.
        processList[i] = initProcess(i, inNums[0], inNums[1], inNums[2]);
        // ^ Init the process based on the inputs.
        switch (inNums[2]) {
            case 1:
                lowPri++;
                break;
            case 2:
                medPri++;
                break;
            case 3:
                hiPri++;
                break;
        }
        buff[0] = '\0';
    }

    // Sorting Processes into their Priority Queues by Arrival Time
    Process lowQueue[lowPri], medQueue[medPri], hiQueue[hiPri];
    int foundList[TOTAL_PROCESSES], found;
    lowPri = 0; medPri = 0; hiPri = 0; // Track the count in each list

    sortProcesses(&processList, &TOTAL_PROCESSES, &foundList, &found);
    for (int i = 0; i < TOTAL_PROCESSES; i++) {
        switch (processList[foundList[i]].PRIORITY) {
            case 1:
                lowQueue[lowPri] = processList[foundList[i]];
                lowPri++;
                break;
            case 2:
                medQueue[medPri] = processList[foundList[i]];
                medPri++;
                break;
            case 3:
                hiQueue[hiPri] = processList[foundList[i]];
                hiPri++;
                break;
        }
        // ^ Depending on the priority of the next process in the sorted foundList
        // processes will be placed in their respective queues.
    }

    // MAIN EXECUTION TICK
    int currentQuantum= 0, QUANTUM = 4, currentPri = 3, time = 0;
    count = 0; // Will be used to count the completed processes
    found = 0; // Will be used to track the number of processes that have been set to running

    while (count < TOTAL_PROCESSES) {
        if (found < TOTAL_PROCESSES && processList[nextProcess].ARRIVAL_TIME == time) {

        }
        // ^ Logic for starting a new process


    }
}