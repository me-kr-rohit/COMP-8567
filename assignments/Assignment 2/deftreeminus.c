/*
Rohit  Kumar(110088741)
COMP:8567
Section:3
References: Manual and other technical forum
*/

#include <sys/wait.h>

#include <dirent.h>

#include <string.h>

#include <stdlib.h>

#include <unistd.h>

#include <time.h>

#include <stdio.h>

#include <stdbool.h>

#include <ctype.h>

int maxAllowedChild[250];
int systemBashPid;
int rohitPid;

// below method will fetch and return parent process id using /proc[PID]/stat
int callFetchParentId(int inputProcessId)
{
    char myFilePath[60];
    FILE *myParentFile;
    int resourceParentPpid;

    sprintf(myFilePath, "/proc/%d/stat", inputProcessId);
    myParentFile = fopen(myFilePath, "r");
    if (myParentFile == NULL)
    {
        printf("--trigger error during file opening--");
        exit(0);
    }
    fscanf(myParentFile, "%*d %*s %*c %d", &resourceParentPpid);
    fclose(myParentFile);
    return resourceParentPpid;
}

// below methods vailadate if a process is having Defunctprocess or not
bool callValidateZombie(int inputProcessId)
{
    char myFilePath[60];
    FILE *myZombieFile;
    char statusFileLine[100];
    char *statusFileVar;
    bool returnStatus = false;
    sprintf(myFilePath, "/proc/%d/status", inputProcessId);
    myZombieFile = fopen(myFilePath, "r");
    if (myZombieFile == NULL)
    {
        printf("--trigger error during file opening--");
        exit(0);
    }
    while (fgets(statusFileLine, sizeof(statusFileLine), myZombieFile) != NULL)
    {
        if (strncmp(statusFileLine, "State:", 6) == 0)
        {
            statusFileVar = statusFileLine + 7;
            if (strstr(statusFileVar, "Z (zombie)") != NULL)
            {
                returnStatus = true; // setting it true if found Zombie
            }
            break;
        }
    }
    fclose(myZombieFile);
    return returnStatus;
}

// below method fetch all the children process for input process
int *callChildrenProcess(int inputProcessId)
{
    char myFilePath[100];
    DIR *rohitDirectory;
    struct dirent *entryStruct;
    int tempPid, tempPPID;
    int counterIncrement = 0;

    sprintf(myFilePath, "/proc/");
    if ((rohitDirectory = opendir(myFilePath)) == NULL)
    {
        printf("Given inputpid is not Correct %s\n", myFilePath);
        return 0;
    }

    while ((entryStruct = readdir(rohitDirectory)) != NULL)
    {
        if (entryStruct->d_type == DT_DIR)
        {
            // ignore non-digit folders
            if (!isdigit(entryStruct->d_name[0]))
                continue;
            tempPid = atoi(entryStruct->d_name);
            // ignore INIT
            if (tempPid == 1)
                continue;
            // ignore Process Itself
            if (tempPid == inputProcessId)
                continue;

            tempPPID = callFetchParentId(tempPid);
            if (tempPPID == inputProcessId)
            {
                maxAllowedChild[counterIncrement] = tempPid;
                counterIncrement++;
            }
        }
    }
    return maxAllowedChild;
    closedir(rohitDirectory);
}

// below method calculates the elapsedTime in min for the given processPID
int callElapsedCalculateTime(int inputProcessId)
{
    char myFilePath[265];
    char *statusFileVar;
    char statusFileLine[265];
    FILE *myElapsedFile;
    int calculateTime;
    struct timespec timeSpecStruct;

    // open the PROC stat file using /PROC/[PID]STAT
    sprintf(myFilePath, "/proc/%d/stat", inputProcessId);
    myElapsedFile = fopen(myFilePath, "r");
    if (myElapsedFile == NULL)
    {
        printf("--trigger error during file opening--\n");
        return -1;
    }

    // calculating the start-in time
    if (fgets(statusFileLine, 100, myElapsedFile) == NULL)
    {
        printf("Error occured in calculating elapsedTime\n");
        fclose(myElapsedFile);
        return -1;
    }
    statusFileVar = strtok(statusFileLine, " ");
    for (int i = 1; i < 22; i++)
    {
        statusFileVar = strtok(NULL, " ");
    }
    calculateTime = atoi(statusFileVar);
    fclose(myElapsedFile);

    // calculating the elapsed-in time using current time
    if (clock_gettime(CLOCK_MONOTONIC, &timeSpecStruct) == -1)
    {
        printf("Triggered error during calculating the time!!");
        return -1;
    }
    return (int)(timeSpecStruct.tv_sec - (calculateTime / sysconf(_SC_CLK_TCK))) / 60;
}

// below method checks if array have value 0
int validateChildren(int childrenArray[])
{

    for (int i = 0; i < 265; i++)
    {
        if (childrenArray[i] > 0)
            return 1;
    }
    return 0;
}

// below method use KILL command to kill process created by user
void killUserProcess(int inputProcessId)
{
    // comparing with globally declared varibale
    if (inputProcessId == systemBashPid)
    {
        printf("bashProcess will be not KILLED\n");
        return;
    }

    // comparing with globally declared varibale
    if (inputProcessId == rohitPid)
    {
        printf("\nNot killed as user provided processId equals to process id %d\n", rohitPid);
        return;
    }
    printf("Process terminated PID- %d\n", inputProcessId);
    // using SIGKILL signals
    kill(inputProcessId, SIGKILL);
}

// extact the process id within the processTree
void getProcessIdInProcessTree(int inputProcessId, int inputDepthLevel, int inputElapsedProcTime,
                               int inputCountDefunct)
{
    int *createChildrenArray;
    int createTempChildrenProcess[265];
    int totalZombieProcess = 0;
    int calculateElapsedTotalTime = 0;
    if (inputDepthLevel >= 265)
    {
        printf("depth level excced\n");
        return;
    }

    // calling callElapsedCalculateTime method
    calculateElapsedTotalTime = callElapsedCalculateTime(inputProcessId);

    // calling callChildrenProcess method
    createChildrenArray = callChildrenProcess(inputProcessId);
    memcpy(createTempChildrenProcess, createChildrenArray,
           265 * sizeof(int));                           // making the copy of child process locally
    memset(maxAllowedChild, 0, sizeof(maxAllowedChild)); // resetting global array declared

    // validate if process has any desendant
    if (!validateChildren(createTempChildrenProcess))
        return;

    // calculate totalZombie desendant
    if (0)
    {
        printf("Desendant is available for inputProcess: %d\n", inputProcessId);
    }
    for (int i = 0; i < 128; i++)
    {
        // ignoring value if it has NULL
        if (createTempChildrenProcess[i] == 0)
            continue;
        if (0)
        {
            printf("%d", createTempChildrenProcess[i]);
        }

        // validate if a process is in Defunct state and then increase the count
        if (callValidateZombie(createTempChildrenProcess[i]))
        {
            totalZombieProcess++;
            if (0)
            {
                printf(" ZombieFound\n");
            }
        }
        else
        {
            if (0)
            {
                printf("\n");
            }
        }
        // Calling Recursively same function
        getProcessIdInProcessTree(createTempChildrenProcess[i], inputDepthLevel + 1, inputElapsedProcTime, inputCountDefunct);
    }

    if (0)
    {
        printf("The Process-PID %d is having %d defunct desendant with %d minutes elapsed-proc time\n",
               inputProcessId, totalZombieProcess, calculateElapsedTotalTime);
    }

    // Terminate/kill the Process if elapsedTime is greater from input
    if (inputElapsedProcTime > 0 && totalZombieProcess > 0)
    {
        if (calculateElapsedTotalTime > inputElapsedProcTime)
        {
            killUserProcess(inputProcessId);
            return;
        }
    }

    // Terminate/kill the Process if Defunct process is greate from input
    else if (inputCountDefunct > 0 && totalZombieProcess > 0)
    {
        if (totalZombieProcess >= inputCountDefunct)
        {
            killUserProcess(inputProcessId);
            return;
        }
    }

    // Terminate/kill all ParentProcess
    else if (inputElapsedProcTime == 0 && inputCountDefunct == 0 && totalZombieProcess > 0)
    {
        killUserProcess(inputProcessId);
        return;
    }

    if (0)
    {
        printf("\nThe inputProcessId: %d is not killed!!\n", inputProcessId);
    }
}

// below is main method and execution starts from here
int main(int parameterPassed, char *parameterValue[])
{

    // declare all the varaibles
    int procElpasedTime = 0, numOfDefunctProcess = 0;
    char *inputOption1;
    char *inputOption2;
    int inputRootPid = 0;

    // checks if synopsis is passed correctly or not
    if (parameterPassed < 2 && parameterPassed > 5)
    {
        printf("deftreeminus [root_process][OPTION1][OPTION2] "
               "[-processid]\n");
        return 1;
    }

    inputRootPid = atoi(parameterValue[1]);
    if (inputRootPid <= 0)
    {
        printf("Error in KeyedIn rootPID\n");
        return 1;
    }

    // checks the inputArgument
    if (parameterPassed == 3)
    {
        rohitPid = atoi(parameterValue[2] + 1);
        if (rohitPid <= 0)
        {
            printf("Error in synopsis. kindly pass option 1 correctly\n");
            return 1;
        }
    }
    else if (parameterPassed == 4)
    {
        inputOption1 = parameterValue[2];
        inputOption2 = parameterValue[3];
    }
    else if (parameterPassed == 5)
    {
        inputOption1 = parameterValue[2];
        inputOption2 = parameterValue[3];
        rohitPid = atoi(parameterValue[4] + 1);
        if (rohitPid <= 0)
        {
            printf("Error in processIDs\n");
            return 1;
        }
    }

    if (parameterPassed > 3)
    {
        if (0)
        {
            printf("Keyed Option1: %s\n", inputOption1);
            printf("Keyed Option2: %s\n", inputOption2);
        }

        if (strcmp(inputOption1, "-t") == 0)
        {
            procElpasedTime = atoi(inputOption2);
            if (procElpasedTime < 1)
            {
                printf("Error in inputProcElpasedTime!\n");
                return 1;
            }
        }
        else if (strcmp(inputOption1, "-b") == 0)
        {
            numOfDefunctProcess = atoi(inputOption2);
            if (numOfDefunctProcess < 1)
            {
                printf("Error in numOfDefunctProcess!\n");
                return 1;
            }
        }
        else
        {
            printf("Error: Kindly keyed option1 correctly!\n");
            return 1;
        }
    }

    // checking and stroing the bash PID here in global varibale
    int systemBashPid = getppid();

    // start searching process tree
    getProcessIdInProcessTree(inputRootPid, 0, procElpasedTime, numOfDefunctProcess);
    return 0;
}