/*
Name: Rohit Kumar
Student Id: 110088741
Section:3
References: Manual and other technical forum
*/
#include <sys/stat.h>

#include <stdio.h>

#include <stdbool.h>

#include <ctype.h>

#include <stdlib.h>

#include <sys/types.h>

#include <unistd.h>

#include <fcntl.h>

#include <string.h>

#include <dirent.h>

// below method will check if a process is defunct or not using /proc[PID]/status
bool fetchDefunctStatus(int inputProcessId)
{
    char filePathStatus[100];
    char linePathStatus[100];
    char *defunctStatus;
    bool defunctResult = false;

    sprintf(filePathStatus, "/proc/%d/status", inputProcessId);
    FILE *myFilePath = fopen(filePathStatus, "r");

    while (fgets(linePathStatus, sizeof(linePathStatus), myFilePath) != NULL)
    {
        if (strncmp(linePathStatus, "State:", 6) == 0)
        {
            defunctStatus = linePathStatus + 7;
            if (strstr(defunctStatus, "Z (zombie)") != NULL)
            {
                defunctResult = true;
            }
            break;
        }
    }

    fclose(myFilePath);
    return defunctResult;
}

// below method will fetch and return parent process id using /proc[PID]/stat
int callFetchParentId(int inputProcessId)
{
    char procPath[200];
    pid_t parentProcessId;

    sprintf(procPath, "/proc/%d/stat", inputProcessId);
    FILE *myFilePath = fopen(procPath, "r");
    if (myFilePath == NULL)
    {
        // printf("Error opening stat file");
        exit(1);
    }

    fscanf(myFilePath, "%*d %*s %*c %d", &parentProcessId);
    fclose(myFilePath);
    return parentProcessId;
}

// below method checks if a given user process id is part of given root process id or not.
void listProcessPidPpid(pid_t inputRootProcessId, pid_t inputProcessId)
{
    pid_t tempPID = inputProcessId;

    while (tempPID != 1)
    {
        if (inputRootProcessId == tempPID)
        {
            pid_t tempPPID = callFetchParentId(inputProcessId);
            printf("Process Id: %d, Parent Id: %d\n", inputProcessId, tempPPID);
            break;
        }
        else
        {
            pid_t tempPPID = callFetchParentId(tempPID);
            tempPID = tempPPID;
        }
    }
}

// below method fetch and print non direct descendants of a given process id using recursion
void listNonDirectDescendantsProcessId(int inputProcessId, int saveOriginalId)
{
    DIR *rohitDirectory;
    struct dirent *entryStruct;
    rohitDirectory = opendir("/proc");
    while ((entryStruct = readdir(rohitDirectory)) != NULL)
    {
        if (entryStruct->d_type == DT_DIR)
        {
            char myFilePath[256];
            snprintf(myFilePath, sizeof(myFilePath), "/proc/%s/stat", entryStruct->d_name);
            FILE *fpn = fopen(myFilePath, "r");
            int tempPID, tempPPID;
            if (fpn != NULL)
            {
                if (fscanf(fpn, "%d %*s %*c %d", &tempPID, &tempPPID) == 2)
                {
                    if (tempPPID == inputProcessId)
                    {
                        pid_t tempPPID1 = callFetchParentId(tempPID);
                        if (tempPPID1 != saveOriginalId)
                        {
                            printf("Process Id: %d\n", tempPID);
                        }
                        // here calling recursively
                        listNonDirectDescendantsProcessId(tempPID, saveOriginalId); 
                    }
                }
                fclose(fpn);
            }
        }
    }
    closedir(rohitDirectory);
}

// below method fetch and print immediate descendants of a given process id using recursion
void listImmediateDescendantsProcessId(int inputProcessId, int saveOriginalId)
{
    DIR *rohitDirectory;
    struct dirent *entryStruct;
    rohitDirectory = opendir("/proc");
    while ((entryStruct = readdir(rohitDirectory)) != NULL)
    {
        if (entryStruct->d_type == DT_DIR)
        {
            char path[256];
            snprintf(path, sizeof(path), "/proc/%s/stat", entryStruct->d_name);
            FILE *fpn = fopen(path, "r");
            int tempPID, tempPPID;
            if (fpn != NULL)
            {
                if (fscanf(fpn, "%d %*s %*c %d", &tempPID, &tempPPID) == 2)
                {
                    if (tempPPID == inputProcessId)
                    {
                        pid_t tempPPID1 = callFetchParentId(tempPID);
                        if (tempPPID1 == saveOriginalId)
                        {
                            printf("Process Id: %d\n", tempPID);
                        }
                        listImmediateDescendantsProcessId(tempPID, saveOriginalId); // here calling recursively
                    }
                }
                fclose(fpn);
            }
        }
    }
    closedir(rohitDirectory);
}

// below method fetch and print sibling process of a give process id
void listSiblingProcessId(int inputProcessId)
{
    DIR *rohitDirectory;
    struct dirent *entryStruct;
    int tempPID, tempPPID;
    rohitDirectory = opendir("/proc");

    while ((entryStruct = readdir(rohitDirectory)) != NULL)
    {
        if (entryStruct->d_type == DT_DIR)
        {
            // ignore non-digit folders
            if (!isdigit(entryStruct->d_name[0]))
                continue;
            tempPID = atoi(entryStruct->d_name);
            // ignore INIT
            if (tempPID == 1)
                continue;
            // ignore Process Itself
            if (tempPID == inputProcessId)
                continue;

            tempPPID = callFetchParentId(tempPID); // fetch PPID by calling  callFetchParentId method

            if (tempPPID == callFetchParentId(inputProcessId))
            {
                printf("Process Id: %d\n", tempPID);
            }
        }
    }
    closedir(rohitDirectory);
}

// below methos fetch and print the sibling of the process that are defunct
void listDefunctSiblingProcessId(int inputProcessId)
{
    DIR *rohitDirectory;
    struct dirent *entryStruct;
    int tempPID, tempPPID;
    rohitDirectory = opendir("/proc");

    while ((entryStruct = readdir(rohitDirectory)) != NULL)
    {
        if (entryStruct->d_type == DT_DIR)
        {
            // ignore non-digit folders
            if (!isdigit(entryStruct->d_name[0]))
                continue;
            tempPID = atoi(entryStruct->d_name);
            // ignore INIT
            if (tempPID == 1)
                continue;
            // ignore Process Itself
            if (tempPID == inputProcessId)
                continue;

            tempPPID = callFetchParentId(tempPID); // fetch PPID by calling  callFetchParentId method
            if (tempPPID == callFetchParentId(inputProcessId) && fetchDefunctStatus(tempPID))
            {
                printf("Process Id: %d\n", tempPID);
            }
        }
    }
    closedir(rohitDirectory);
}

// below method fetch and list the grand children process of given process id
void listGrandchildrenProcessId(int inputProcessId)
{
    DIR *rohitDirectory;
    struct dirent *entryStruct;
    int tempPID, tempPPID;
    rohitDirectory = opendir("/proc");

    while ((entryStruct = readdir(rohitDirectory)) != NULL)
    {
        if (entryStruct->d_type == DT_DIR)
        {
            // ignore non-digit folders
            if (!isdigit(entryStruct->d_name[0]))
                continue;
            tempPID = atoi(entryStruct->d_name);
            // ignore INIT
            if (tempPID == 1)
                continue;
            // ignore Process Itself
            if (tempPID == inputProcessId)
                continue;

            tempPPID = callFetchParentId(tempPID); // fetch PPID by calling  callFetchParentId method
            if (tempPPID > 1 && callFetchParentId(tempPPID) == inputProcessId)
            {
                printf("Process Id: %d\n", tempPID);
            }
        }
    }
    closedir(rohitDirectory);
}

// below method checks and print if given process is defunct ot non-defunct
void listDefunctStatusProcessId(int inputProcessId)
{
    if (fetchDefunctStatus(inputProcessId))
    {
        printf("Process is: %d is in DEFUNCT state\n", inputProcessId);
    }
    else
    {
        printf("Process is: %d is not in DEFUNCT state\n", inputProcessId);
    }
}

// below method list all the direct desendants that are in defunct state
void listDefunctDirectDesendantsProcessId(int inputProcessId)
{
    DIR *rohitDirectory;
    struct dirent *entryStruct;
    int tempPID, tempPPID;
    rohitDirectory = opendir("/proc");

    while ((entryStruct = readdir(rohitDirectory)) != NULL)
    {
        if (entryStruct->d_type == DT_DIR)
        {
            // ignore non-digit folders
            if (!isdigit(entryStruct->d_name[0]))
                continue;
            tempPID = atoi(entryStruct->d_name);
            // ignore INIT
            if (tempPID == 1)
                continue;
            // ignore Process Itself
            if (tempPID == inputProcessId)
                continue;

            tempPPID = callFetchParentId(tempPID); // fetch PPID by calling  callFetchParentId method
            if (tempPPID == inputProcessId && fetchDefunctStatus(tempPID))
            {
                printf("Process Id: %d\n", tempPID);
            }
        }
    }
    closedir(rohitDirectory);
}

//below is the main method and execution starts from here
int main(int parameterPassed, char *parameterValue[])
{
    int index = 0;

    // below logic checks if parameter passed by user is in correct format or not.
    if (parameterPassed < 3 || parameterPassed > 8)
    {
        printf("----CALL PRCINFO AS BELOW----\n");
        printf("prcinfo [root_process] [process_id1] [process_id2]… [process_id(n)] [OPTION] --");
        printf("where n is greater than 0 and less than 6\n");
        exit(1);
    }

    // below logic checks and set the index if OPTION is provided by then ignore then OPTION
    if (atoi(parameterValue[parameterPassed - 1]) == 0)
    {
        printf("OPTION is provided by user\n");
        index = parameterPassed - 3;
    }
    else
    {
        printf("OPTION is not provided by user\n");
        index = parameterPassed - 2;
    }

    // below logic validated if given process id belong to the same given root process or not
    for (int i = 2; i < index + 2; i++)
    {
        listProcessPidPpid(atoi(parameterValue[1]), atoi(parameterValue[i]));
    }

    // below logic checks the option and invoke respective method accordingly
    if (strcmp(parameterValue[parameterPassed - 1], "-nd") == 0)
    {
        printf("Non Direct Descendants of input ProcessId: %d are:\n", atoi(parameterValue[2]));
        listNonDirectDescendantsProcessId(atoi(parameterValue[2]), atoi(parameterValue[2]));
    }

    if (strcmp(parameterValue[parameterPassed - 1], "-dd") == 0)
    {
        printf("Immediate Descendants of ProcessId: %d are:\n", atoi(parameterValue[2]));
        listImmediateDescendantsProcessId(atoi(parameterValue[2]), atoi(parameterValue[2]));
    }

    if (strcmp(parameterValue[parameterPassed - 1], "-sb") == 0)
    {
        printf("lists the PIDs of all the sibling processes of process id1: %d\n", atoi(parameterValue[2]));
        listSiblingProcessId(atoi(parameterValue[2]));
    }

    if (strcmp(parameterValue[parameterPassed - 1], "-sz") == 0)
    {
        printf("Lists the PIDs of all sibling processes of process id1: %d that are defunct\n", atoi(parameterValue[2]));
        listDefunctSiblingProcessId(atoi(parameterValue[2]));
    }

    if (strcmp(parameterValue[parameterPassed - 1], "-gc") == 0)
    {
        printf("lists the PIDs of all the GrandChildren processes of process id1: %d\n", atoi(parameterValue[2]));
        listGrandchildrenProcessId(atoi(parameterValue[2]));
    }

    if (strcmp(parameterValue[parameterPassed - 1], "-zz") == 0)
    {
        printf("lists the status of process_id1 (Defunct/ Not Defunct) of process id1: %d\n", atoi(parameterValue[2]));
        listDefunctStatusProcessId(atoi(parameterValue[2]));
    }

    if (strcmp(parameterValue[parameterPassed - 1], "-zc") == 0)
    {
        printf("lists the PIDs of of all the direct defunct descendants of process id1: %d\n", atoi(parameterValue[2]));
        listDefunctDirectDesendantsProcessId(atoi(parameterValue[2]));
    }
    exit(0);
}