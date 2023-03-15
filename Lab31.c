#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Turnover time, set the arrival time to the time unit that the process starts.
 * Default set to -1. If AT > -1, it has been started.
 *
 * LOGIC:
 * PROCESS COUNTING and REGISTRATION:
 * Take, as input, the number of processes
 * Prepare next process cache to point to the first index of ProcessList (set to 0 for index 1)
 *
 * SETTING EACH PROCESSES CONDITIONS:
 * loop through those processes and add them to a list of all processes according to input
 * > if the inputted arrival time is less than the arrival time of the process in the index of nextProcess's value and != 0
 * >> nextProcess = the newly made process
 * > If the arrival time entered is 0
 * >> immediately add it to the readyQueue and set RUNNING = 1
 *
 * MAIN EXECUTION TICK:
 * While the number of completed processes is less than the TOTAL_PROCESSES, start and continue exec loop
 * Increment time
 *
 *  --> Check if there is a new process getting appended <--
 * > If nextProcess is found to have ARRIVAL_TIME = time and RUNNING = 0
 * >> Set RUNNING = 1 and Add next process to readyQueue
 * >> If all readyQueue Process are completed
 * >>> Set CURRENT_PROCESS to the newly added process to immediately start exec
 * >> Call getNextProcess(int* processes) method
 *
 *  --> Execute the process to the time quantum or end of remaining time <--
 * if currentQuantum == QUANTUM
 * > Move to next process
 * If the current process.RUNNING = -1
 * > Cycle through readyqueue until we find a process that isn't complete
 *  If program has remaining time and RUNNING == 1
 * > Decrement remaining time
 *
 * -- > If the REMAINING_TIME becomes 0 here then it has finished and we can move on < --
 * > if current process will finish
 * >> set running to -1 and increment the completed count
 * >> Set current process to the next in the queue or to index 0 if we are the end of queue
 *
 * Print the execution message
 * increment the current quantum
 */

typedef struct {
    int RUNNING;
    int PID;
    int ARRIVAL_TIME;
    int REMANING_TIME;
    int COMPLETION_TIME;
    // ^ This is initially the burst time before it's first cycle. There is no need to waste memory keeping the
} Process;
// ^ The struct representing the faux process.

Process initProcess(int PID, int AT, int RT) {
    Process p;
    p.RUNNING = 0;
    p.PID = PID;
    p.ARRIVAL_TIME = AT;
    p.REMANING_TIME = RT;
    return p;
}
// ^ General struct initialization method to start the struct with defined values.

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
// ^ A method that takes in a buffer of possible numbers and, if all are numbers,
// the int array that was passed into the method will have it's values set to the numbers in the buffer.

int getNextProcess(Process *processes, const int *TOTAL) {
    int nextProcess = -1;
    for (int i = 0; i < *TOTAL; i++) {
        if (processes[i].RUNNING == 0 && (nextProcess == -1 || processes[nextProcess].ARRIVAL_TIME > processes[i].ARRIVAL_TIME))
            nextProcess = i;
    }
    return nextProcess;
}

int main() {
    char buff[80];
    int buffatoi;

    // PROCESS COUNTING AND REGISTRATION
    int TOTAL_PROCESSES = 20;
    printf("Enter the number of processes (20 or less): ");
    fgets(buff, sizeof(buff), stdin);
    if (!isDigit(buff)) {
        printf("Number entered was detected to not be a valid integer. Aborting.");
        return -1;
    }
    buffatoi = atoi(buff);
    // ^ Characters entered were detected to not be numbers.
    TOTAL_PROCESSES = (buffatoi >= 0 && buffatoi <= 20) ? buffatoi : -1;
    if (TOTAL_PROCESSES == -1) {
        printf("Number entered was detected to not be within 0 to 20. Aborting.");
        return -2;
    }
    // ^ Integer entered was not from 0 to 20.
    Process processList[TOTAL_PROCESSES];
    Process readyQueue[TOTAL_PROCESSES];
    // ^ A list of all processes and then a readyQueue for all processes in the round-robin.

    // SETTING EACH PROCESSES CONDITIONS
    int nextProcess = 0, count, input[2], readyQueuePos = 0;
    // ^ Saving the index of the next process to lessen list iteration.
    for (int i = 0; i < TOTAL_PROCESSES; i++) {
        printf("Enter the Arrival Time and Total Burst Time for process %d seperated by a space: ", i);
        fgets(buff, sizeof(buff), stdin);
        count = parseSpaceSeperated(buff, &input);
        // ^ Parse the space seperate arrival and running time for the process.
        if (count != 2) {
            printf("Improper input detected. Did not enter exactly 2 arguments. Aborting");
            return -3;
        }
        // ^ If there aren't exactly 2 inputs, one for AT and one for RT, then abort the program.
        processList[i] = initProcess(i, input[0], input[1]);
        // ^ If input was valid, set the PID to the current loop, AT to the first input, and RT to the second.
        if (processList[i].ARRIVAL_TIME < processList[nextProcess].ARRIVAL_TIME) {
            nextProcess = i;
        }
        if (input[0] == 0) {
            readyQueue[readyQueuePos] = processList[i];
            readyQueuePos++;
        }
        // ^ If the AT of the program was set to 0, add it to the queue.
        buff[0] = '\0';
        // ^ Put nullterm at the first index of the buffer to "clear" the buffer.
    }

    printf("Execution order is listed vertically with the current time to the left of the process '<time> | Process <#>' | Remaining time\n For Example:\n");
    printf("15 | Process 7 | 23\n\n\n <--- EXECUTION START ---> \n");

    // MAIN EXECUTION TICK
    int currentQuantum = 0, QUANTUM = 4, CURRENT_PROCESS = 0, completed = 0, readyQueueSize = 0, time = -1;
    while (completed < TOTAL_PROCESSES) { // Main loop
        time++;
        // CHECK IF NEW PROCESS IS GETTING APPENDED
        if (time == processList[nextProcess].ARRIVAL_TIME) {
            // ^ If time quantum equals the arrival time of next process...
            processList[nextProcess].RUNNING = 1;
            readyQueue[readyQueueSize] = processList[nextProcess];
            if (completed == readyQueueSize)
                CURRENT_PROCESS = readyQueueSize;
            // ^ If all current readyQueue processes are completed, make sure we start the execution on the new process immediately.
            readyQueueSize++;
            nextProcess = getNextProcess(processList, &TOTAL_PROCESSES);
            // ^ Set the process to running so that getting the next process is easier,
            // then add the process to the readyQueue,
            // then increment the readyQueuePos for the next time this runs.
            // then get the next process,
        }
        
        if ((readyQueueSize) == 0 || completed == readyQueueSize)
            continue;

        // EXECUTE THE PROCESS TO TIME QUANTUM OR REMAINING_TIME == 0
        if (currentQuantum == QUANTUM) {
            CURRENT_PROCESS = (CURRENT_PROCESS >= readyQueueSize - 1) ? 0 : CURRENT_PROCESS + 1;
            currentQuantum = 0;
        }
        // ^ When the currentQuantum reaches the max quantum, swap processes and reset the currentQuantum.
        while (readyQueue[CURRENT_PROCESS].RUNNING == -1)
            CURRENT_PROCESS = (CURRENT_PROCESS >= readyQueueSize-1) ? 0 : CURRENT_PROCESS + 1;
        // ^ Prevent skipping during the discovery of completed processes.
        if (readyQueue[CURRENT_PROCESS].RUNNING == 1 
            && readyQueue[CURRENT_PROCESS].REMANING_TIME > 0) {
            // ^ If the current process is running, have a positive remaining time, and the current quantum is within
            // the quantum limit, run the program another cycle.
            readyQueue[CURRENT_PROCESS].REMANING_TIME--;
            // Increment the total time, the current quantum, and decrement the processes remaining time.
            if (readyQueue[CURRENT_PROCESS].REMANING_TIME == 0) {
                readyQueue[CURRENT_PROCESS].RUNNING = -1; readyQueue[CURRENT_PROCESS].COMPLETION_TIME = time;
                completed++;
                CURRENT_PROCESS = (CURRENT_PROCESS >= readyQueueSize-1) ? 0 : CURRENT_PROCESS + 1;
                // ^ If the programs remaining time is now 0, set RUNNING to -1 (meaning completed) and
                // increment the number of completed processes
            }
            printf("%d | Process %d | %d\n", time, readyQueue[CURRENT_PROCESS].PID, readyQueue[CURRENT_PROCESS].REMANING_TIME);
        }
        currentQuantum++;
        // ^ Above this will be the remaining time decrements and such from the ready que along with the time vs quantum counter
    }

    printf("\n\n <--- Turnover Times ---> \n");
    for (int i = 0; i < TOTAL_PROCESSES; i++) {
        printf("Process %d Turnover: %d\n", readyQueue[i].PID, (readyQueue[i].COMPLETION_TIME - readyQueue[i].ARRIVAL_TIME));
    }
    return 0;
}
