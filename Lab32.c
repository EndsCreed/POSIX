#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Explanation of logic:
 *
 * This is a bog standard priority queue which does not round robin.
 * Processes are entered one by one in the beginning and then sorted with quicksort by their arrival time.
 * Once they are sorted by arrival time, they are added in this sorted order into their priority queues.
 * From this point on, determining if we change process will be up to whether there is a process with a higher
 * priority that can run or if the running process finishes. We look at the first non-completed process of every
 * queue and check if their arrival time is less than the current total program running time.
 * If it is, that becomes the next process, if not, we move on to a lower priority queue and do the same check.
 * If none of the 3 processes can run, we return a dummy process with a PID of -1.
 * If this process is returned, we attempt to keep running the current process. If the current process is completed,
 * we will keep running an empty loop until a new process arrives which will then become the currentProcess.
 * When all processes are finished we will print their Start Time, Turnover Time, and their priority.
 * The goal is to see that higher priority processes have a smaller turnover on average while lower priority
 * processes either have a much higher turnover from being interrupted or they start much later than they arrive.
 * This highlight the strengths and weaknesses of a standard priority queue without an aging system.
 *
 * This lab makes use of structs for the purpose of tracking properties of each process and tracking the live
 * state of the virtual system which is created and running said "processes".
 */

typedef struct {
    short RUNNING; // Either -1, 0, or 1 for Completed, Standby, Running
    int PID;
    int ARRIVAL_TIME; // Time unit the process was "started"
    int START_TIME; // When the process was actually allowed to start
    int REMAINING_TIME; // The amount of time units the process needs to finish execution
    int PRIORITY; // 1, 2, or 3 for Low, Medium, High
    int COMPLETION_TIME; // When the process managed to finish execution
} Process;

typedef struct { // Kind of an active data storage?
    Process *PROCESSLIST; // Main list of all processes. Is eventually sorted with quicksort.
    Process **HIQUEUE; //--|
    Process **MEDQUEUE; // | -> Double pointers representing the high, medium, and low process priority queues.
    Process **LOWQUEUE; //-|
    // vv Save the array sizes and how many process are completed in each array vv
    int PROCESSSIZE; // Size of the PROCESSLIST
    int HISIZE, HICOMP; // Size of the HIQUEUE and how many processes in it are completed
    int MEDSIZE, MEDCOMP; //
    int LOWSIZE, LOWCOMP;
} SysState;

SysState sysState; // Yes... I know.... A taboo global variable. Leave me alone...

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
// ^ General Util method

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
// ^ General Util method for taking multi inputs in 1 line.

Process *getNextProcess(int *time) {
    int index = 0;
    Process *next;

    if (*time == 0) {
        *time = sysState.HIQUEUE[0]->ARRIVAL_TIME;
        *time = (sysState.MEDQUEUE[0]->ARRIVAL_TIME < *time) ? index = 1, sysState.MEDQUEUE[0]->ARRIVAL_TIME : *time;
        *time = (sysState.LOWQUEUE[0]->ARRIVAL_TIME < *time) ? index = 2, sysState.LOWQUEUE[0]->ARRIVAL_TIME : *time;

        switch (index) {
            case 0: return sysState.HIQUEUE[0];
            case 1: return sysState.MEDQUEUE[0];
            case 2: return sysState.LOWQUEUE[0];
        }
    }
    // ^ Conditional to check if the main execution loop just started, if it did, we'll skip the time to the closest
    // process in terms of arrival time whilst respecting priority. If two of the same arrival times are found at
    // different priorities, it will take the highest priority and return it.

    if (sysState.HICOMP != sysState.HISIZE) {
        index = 0;
        while (index < sysState.HISIZE - 1 && sysState.HIQUEUE[index]->RUNNING == -1) {
            index++;
        }
        next = sysState.HIQUEUE[index];
        if (next->ARRIVAL_TIME <= *time && next->RUNNING != -1)
            return next;
    }
    if (sysState.MEDCOMP != sysState.MEDSIZE) {
        index = 0;
        while (index < sysState.MEDSIZE - 1 && sysState.MEDQUEUE[index]->RUNNING == -1) {
            index++;
        }
        next = sysState.MEDQUEUE[index];
        if (next->ARRIVAL_TIME <= *time && next->RUNNING != -1)
            return next;
    }
    if (sysState.LOWCOMP != sysState.LOWSIZE) {
        index = 0;
        while (index < sysState.LOWSIZE - 1 && sysState.LOWQUEUE[index]->RUNNING == -1) {
            index++;
        }
        next = sysState.LOWQUEUE[index];
        if (next->ARRIVAL_TIME <= *time && next->RUNNING != -1)
            return next;
    }
    return createDummyProcess();
}
// ^ This method will only return a process if it is able to be ran at the current system time. Else if returns
// and "invalid" dummy process with a negative PID as a flag for it being said dummy process. Tests will be done
// on the main execution tick to test if the process returned by this method is a real process or the placeholder dummy
// process.

void swap(Process *a, Process *b) {
    Process t = *a;
    *a = *b;
    *b = t;
}
// ^ Simple swap method for the quick sort.

int split(Process array[], int low, int high) {
    Process pivot = array[high];
    // ^ Pivot will be the last index

    int p = low;
    // ^ counts the number of processes with a lower arrival time
    // Used as a placeholder for where to perform swaps to move elements to the left
    // Will be the final split point

    for (int i = low; i < high; i++) {
        if (array[i].ARRIVAL_TIME < pivot.ARRIVAL_TIME) {
            swap(&array[p], &array[i]);
            p++;
        }
        // ^ As long as we're one less than array size:
        // If the current loop index arrival time is less than the pivots, move the loops process to the left
        // (after the last process that was moved) and increment p for the next possible swap
    }

    swap(&array[p], &array[high]);
    // ^ Move the pointer to the spot after any previous swaps

    return p;
}
// ^ Using the last index as the pivot, we set a "pointer" (really just an index number counter) to the first element.
// If we find a element larger than the pivot, we move that element to where the "index pointer" is and move the index
// pointer to the next index. Once we reach the end of the array, the index pointer will be on at the index after the
// last moved element. We then swap the element at the pointer with the pivot and return the index of the pivot to
// be used in the next batch of quick sort. This servers as the "split" or "partition" point of the array.

void quickSortProcesses(int low, int high) {
    if (low < high ) {
        int pi = split(sysState.PROCESSLIST, low, high);
        // ^ The split point returned by the split function.

        quickSortProcesses(low, pi - 1);
        // Sort left
        quickSortProcesses(pi + 1, high);
        // Sort right
    }
}
// ^ After getting the split point of the

int main() {
    char buff[80];
    int buffatoi;
    // ^ Vars ^

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
    // -----------------------------------------------------------------------

    // SETTING EACH PROCESSES PROPERTIES
    sysState.PROCESSLIST = malloc(sizeof(Process)*sysState.PROCESSSIZE);
    int inNums[3], count;
    // ^ Vars ^

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
    // --------------------------------------------------------------------

    // Sorting Processes into their Priority Queues by Arrival Time
    sysState.LOWQUEUE = malloc(sizeof(Process)*sysState.LOWSIZE);
    sysState.MEDQUEUE = malloc(sizeof(Process)*sysState.MEDSIZE);
    sysState.HIQUEUE = malloc(sizeof(Process)*sysState.HISIZE);
    sysState.LOWSIZE = 0; sysState.MEDSIZE = 0; sysState.HISIZE = 0; // Track the count in each list
    // ^ Vars ^

    quickSortProcesses(0, sysState.PROCESSSIZE-1);
    // ^ Sort the process list in order or arrival time.
    for (int i = 0; i < sysState.PROCESSSIZE; i++) {
        switch (sysState.PROCESSLIST[i].PRIORITY) {
            case 1:
                sysState.LOWQUEUE[sysState.LOWSIZE] = &sysState.PROCESSLIST[i];
                sysState.LOWSIZE++;
                break;
            case 2:
                sysState.MEDQUEUE[sysState.MEDSIZE] = &sysState.PROCESSLIST[i];
                sysState.MEDSIZE++;
                break;
            case 3:
                sysState.HIQUEUE[sysState.HISIZE] = &sysState.PROCESSLIST[i];
                sysState.HISIZE++;
                break;
        }
        // ^ Depending on the priority of the next process in process list
        // Processes will be placed in their respective queues. They will automatically be have their queues
        // Sorted by arrival time since the original process list should have been sorted by arrival time
    }
    // --------------------------------------------------------------------------------------

    // MAIN EXECUTION TICK
    int currentQuantum= 0, QUANTUM = 4, time = 0, running = 0;
    sysState.LOWCOMP = 0; sysState.MEDCOMP = 0; sysState.HICOMP = 0;
    Process *currentProcess;
    // ^ Vars ^

    while ((sysState.LOWCOMP + sysState.MEDCOMP + sysState.HICOMP) < sysState.PROCESSSIZE) {
        if (getNextProcess(&time)->PID != -1) {
            currentProcess = getNextProcess(&time);
            if (currentProcess->RUNNING == 0) {
                currentProcess->START_TIME = time;
                currentProcess->RUNNING = 1;
                running++;
            }
        }
        // ^ Logic for setting the current process and finding the next process.
        // If the next processes arrival time is currentTime or less, we haven't found every process
        // If the amount of found processes is equal to the total, we don't need to search for a next process anymore.
        while (currentProcess->REMAINING_TIME > 0) {
            if (currentQuantum == QUANTUM) {
                currentQuantum = 0;
                if (getNextProcess(&time)->PRIORITY > currentProcess->PRIORITY)
                    break;
            }
            currentProcess->REMAINING_TIME--;
            time++; currentQuantum++;
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
        }
        time++; currentQuantum++;
        // ^ Out of loop increment serves to increment the time and quantum if there is no elgible current process.
    }
    // ----------------------------------------------------------------------

    // FINISHED
    for (int i = 0; i < sysState.PROCESSSIZE; i++) {
        printf("Process %d Start Time: %d (Arrival %d) | Turnover: %d | Priority: %d\n",
                        sysState.PROCESSLIST[i].PID,
                        sysState.PROCESSLIST[i].START_TIME,
                        sysState.PROCESSLIST[i].ARRIVAL_TIME,
                        (sysState.PROCESSLIST[i].COMPLETION_TIME - sysState.PROCESSLIST[i].START_TIME),
                        sysState.PROCESSLIST[i].PRIORITY);
    }
}