#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Explanation of logic:
 *
 * This is a bog standard priority queue which does not round robin.
 * Processes are entered one by one in the beginning and then sorted by their arrival time.
 * Once they are sorted by arrival time, they are added in this sorted order into their priority queues.
 * From this point on, determining if we change process will be up to whether there is a process with a higher
 * priority that can run. We look at the first non-completed process of every queue and check if their arrival
 * time is less than the current total program running time. If it is, that becomes the next process, if not, we move
 * on to a lower priority queue and do the same check. If none of the 3 processes can run, we return a dummy
 * process with a PID of -1. If this process is returned, we attempt to keep running the current process. If the
 * current process is completed, we will keep running an empty loop until a new process arrives. We could find what the
 * next future process in this instance is. If there are no processes left to find, we will return a dummy process with
 * PID of -2 to show this and then keep looping through the last process until we are done.
 */

typedef struct {
    short RUNNING;
    int PID;
    int ARRIVAL_TIME;
    int START_TIME;
    int REMAINING_TIME;
    int PRIORITY;
    int COMPLETION_TIME;
} Process;

typedef struct {
    Process *PROCESSLIST;
    Process *HIQUEUE;
    Process *MEDQUEUE;
    Process *LOWQUEUE;
    // vv Save the array sizes and how many process are completed in each array vv
    int PROCESSSIZE;
    int HISIZE, HICOMP;
    int MEDSIZE, MEDCOMP;
    int LOWSIZE, LOWCOMP;
} SysState;

SysState sysState;

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

Process *findByPID(int *PID) {
    Process *p;
    for (int i = 0; i < sysState.PROCESSSIZE; i++) {
        if (sysState.PROCESSLIST[i].PID == *PID)
            return &sysState.PROCESSLIST[i];
    }
    return createDummyProcess();
}

Process *getNextProcess(int *time) {
    int index = 0;

    while (index < sysState.HISIZE) {
        if (sysState.HIQUEUE[index].RUNNING == -1) {
            index++;
            continue;
        }

        return findByPID(&sysState.HIQUEUE[index].PID);
    }
    // ^ Return first non-completed process in hiQueue with an arrival time less than current time.
    // We don't need to check the lower priority lists because they are lower priority...
    index = 0;
    while (index < sysState.MEDSIZE) {
        if (sysState.MEDQUEUE[index].RUNNING == -1) {
            index++;
            continue;
        }
        return findByPID(&sysState.MEDQUEUE[index].PID);
    }
    // ^ Set next to first non-completed process in medQueue with an arrival time less than current time.
    // ^ We don't need to consider the result of
    index = 0;
    while (index < sysState.LOWSIZE) {
        if (sysState.LOWQUEUE[index].RUNNING == -1) {
            index++;
            continue;
        }

        return findByPID(&sysState.LOWQUEUE[index].PID);
    }
    // ^ Scan through each list starting with High priority and return the first process in the lists that isn't
    // completed with the highest priority.
    // Since the priority queues are already sorted, I don't need to consider arrival times of each process in relation
    // to each other. I only need to get the first process in each list that isn't completed since that is guaranteed to
    // be the lowest (or equally lowest) arrival time from the rest in the same list.
    // Only three processes will be found in the worst case. From the three that are found to be under the current time,
    // we take the highest priority and return it as we find it. If none are found, we return a dummy process.
}
void swap(Process *a, Process *b) {
    Process t = *a;
    *a = *b;
    *b = t;
}
int split(Process array[], int low, int high) {
    Process pivot = array[high];

    int p = low;

    for (int i = low; i < high; i++) {
        if (array[i].ARRIVAL_TIME > pivot.ARRIVAL_TIME) {
            swap(&array[i], &array[p]);
            p++;
        }
    }
}
void quickSortProcesses(int *pidArry, int size)

void sortProcesses(int *foundList, int *found) {
    int next, present = 0;
    while (sysState.PROCESSSIZE > *found) {
    // ^ While we haven't found all processes and ordered them
        next = 0;
        for (int i = 0; i < sysState.PROCESSSIZE; i++) {
            // ^ While the current iteration is under the total count of processes and we haven't found every process
            for (int z = 0; z < *found; z++) {
                present = (sysState.PROCESSLIST[i].PID == sysState.PROCESSLIST[found[z]].PID) ? 1 : present;
                // ^ On each found process, check if the PID of a prev found process matches the current new "best" process
                // ^ If it does match, set "present" to 1, otherwise, keep it at what it was set to before. (0 if not found yet or 1 if it has been found)
            }
            next = (!present && (sysState.PROCESSLIST[i].ARRIVAL_TIME < sysState.PROCESSLIST[next].ARRIVAL_TIME)) ? i : next;
            // ^ If the process was found in the found list,
        }
        foundList[*found] = next;
        // ^ After we reach the end of the list of processes, add the found next best to the found list at the index of the total
        // number of found processes. Since array index's start at 0 but we count from 1, this should add the process to the
        // index before the total found count. Avoiding the off-by-one error.
        *found = *found + 1;
        // ^ Increment the found count
    }
}

int main() {
    char buff[80];
    int buffatoi;

    // GETTING NUMBER OF PROCESSES
    printf("Enter the number of process (Max 20): ");
    fgets(buff, sizeof(buff), stdin);

    if (!isDigit(buff))
        return -1;
    // ^ If inputted number of process is not a valid number, exit.

    buffatoi = atoi(buff);
    buff[0] = '\0'; // Empty Buffer
    sysState.PROCESSSIZE = (buffatoi >= 0 && buffatoi <= 20) ? buffatoi : -1;
    if (sysState.PROCESSSIZE == -1)
        return -2;
    // ^ If the number of processes entered is not from 0 to 20, exit.

    // SETTING EACH PROCESSES PROPERTIES
    sysState.PROCESSLIST = malloc(sizeof(Process)*sysState.PROCESSSIZE);
    int inNums[3], count;


    for (int i = 0; i < sysState.PROCESSSIZE; i++) {
        printf("Enter the Arrival Time, Burst Time, and Priority of seperated by spaces for Process %d", i);
        fgets(buff, sizeof(buff), stdin);
        count = parseSpaceSeperated(buff, &inNums);
        // ^ Set inputs into the Input Numbers array.

        if (count != 3)
            return -3;
        // ^ If count is not the right amount of inputs, exit.
        sysState.PROCESSLIST[i] = initProcess(i, inNums[0], inNums[1], inNums[2]);
        // ^ Init the process based on the inputs.
        switch (inNums[2]) {
            case 1:
                sysState.LOWSIZE++;
                break;
            case 2:
                sysState.MEDSIZE++;
                break;
            case 3:
                sysState.HISIZE++;
                break;
        }
        buff[0] = '\0';
    }

    // Sorting Processes into their Priority Queues by Arrival Time
    sysState.LOWQUEUE = malloc(sizeof(Process)*sysState.LOWSIZE);
    sysState.MEDQUEUE = malloc(sizeof(Process)*sysState.MEDSIZE);
    sysState.HIQUEUE = malloc(sizeof(Process)*sysState.HISIZE);
    int foundList[sysState.PROCESSSIZE], found = 0;
    sysState.LOWSIZE = 0; sysState.MEDSIZE = 0; sysState.HISIZE = 0; // Track the count in each list

    sortProcesses(&foundList, &found);
    for (int i = 0; i < sysState.PROCESSSIZE; i++) {
        switch (sysState.PROCESSLIST[foundList[i]].PRIORITY) {
            case 1:
                sysState.LOWQUEUE[sysState.LOWSIZE] = sysState.PROCESSLIST[foundList[i]];
                sysState.LOWSIZE++;
                break;
            case 2:
                sysState.MEDQUEUE[sysState.MEDSIZE] = sysState.PROCESSLIST[foundList[i]];
                sysState.MEDSIZE++;
                break;
            case 3:
                sysState.HIQUEUE[sysState.HISIZE] = sysState.PROCESSLIST[foundList[i]];
                sysState.HISIZE++;
                break;
        }
        // ^ Depending on the priority of the next process in the sorted foundList
        // processes will be placed in their respective queues.
    }

    // MAIN EXECUTION TICK
    int currentQuantum= 0, QUANTUM = 4, time = 0;
    sysState.LOWCOMP = 0; sysState.MEDCOMP = 0; sysState.HICOMP = 0;
     // Number of completed processes in each list
    found = 0; // Will be used to track the number of processes that have been set to running
    Process *currentProcess = getNextProcess(&time);
    time = currentProcess->ARRIVAL_TIME; // Skip to first process execution
    currentProcess->RUNNING = 1; // Current Process is now running
    found++; // Increment the number of found processes

    Process *nextProcess = getNextProcess(&time);

    while ((sysState.LOWCOMP + sysState.MEDCOMP + sysState.HICOMP) < sysState.PROCESSSIZE) {
        if (nextProcess->ARRIVAL_TIME <= time && found < sysState.PROCESSSIZE
            && nextProcess->PRIORITY > currentProcess->PRIORITY) {
            nextProcess->RUNNING = 1;
            currentProcess = nextProcess;
            currentProcess->START_TIME = time;
            nextProcess = getNextProcess(&time);
            found++;
        }
        // ^ Logic for setting the current process and finding the next process.
        // If the next processes arrival time is currentTime or less, we haven't found every process
        // If the amount of found processes is equal to the total, we don't need to search for a next process anymore.
        while (currentProcess->REMAINING_TIME > 0) {
            if (currentQuantum == QUANTUM) {
                currentQuantum = 0;
                if (nextProcess->PRIORITY > currentProcess->PRIORITY)
                    break;
            }
            currentProcess->REMAINING_TIME--;
            if (currentProcess->REMAINING_TIME == 0) {
                currentProcess->RUNNING = -1;
                currentProcess->COMPLETION_TIME = time;
                switch (currentProcess->PRIORITY) {
                    case 3: sysState.HICOMP++; break;
                    case 2: sysState.MEDCOMP++; break;
                    case 1: sysState.LOWCOMP++; break;
                }
                // ^ Increment the completed amount of the appropriate queue
                break;
            }
            // ^ If the remaining time is 0, break out of the loop so we rely on the out of loop time increment.
            time++; currentQuantum++;
        }
        time++; currentQuantum++;
        // ^ Out of loop increment serves to increment the time and quantum if there is no elgible current process.
    }

    for (int i = 0; i < sysState.PROCESSSIZE; i++) {
        printf("Process %d Start Time: %d | Turnover: %d | Priority: %d",
                        sysState.PROCESSLIST[i].PID,
                        sysState.PROCESSLIST[i].START_TIME,
                        (sysState.PROCESSLIST[i].COMPLETION_TIME - sysState.PROCESSLIST[i].START_TIME),
                        sysState.PROCESSLIST[i].PRIORITY);
    }
}