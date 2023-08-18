#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

void main()
{    
    int num1 = 10, num2 = 20, num3 = 30;

    umask(0);
    int textFile = open("sample.txt", O_RDWR | O_CREAT, 0777);

    if (textFile == -1) 
    {
        printf("\nError occured while opening file");
        exit(1);
    }

    int process1, process2;

    process1 = fork();
    process2 = fork();

    if(process1 == 0)
    {
        if(process2 == 0)
        {
            printf("The process id of process2 is: %d\n", getpid());
            char* message = "COMP 8567\n";
            write(textFile, message, strlen(message));

            int i;
            for (i = 0; i < 15; i++) 
            {
                printf("\nchild process: process id = %d, parent process id = %d\n", getpid(), getppid());
                sleep(1);
            }
            num1 += 10;
            num2 += 10;
            num3 += 10;
            printf("\nchild Process: num1 = %d, num2 = %d, num3 = %d", num1, num2, num3);
            exit(0);
        }
        else if (process2 >0 )
        {
            printf("First child process id : %d\n", getpid());
            char* message = "COMP 8567\n";
            write(textFile, message, strlen(message));

            int i;
            for (i = 0; i < 15; i++) 
            {
                printf("\nchild process: process id = %d, parent process id = %d\n", getpid(), getppid());
                sleep(1);
            }

            num1 += 10;
            num2 += 10;
            num3 += 10;

            printf("\nchild process: num1 = %d, num2 = %d, num3 = %d\n", num1, num2, num3);

            exit(0);
        }
        else
        {
            printf("\n!!Error Ocuured in Fork!!\n");
        }
    } 
    else if (process1 > 0)
    {
        if(process2 == 0)
        {
            printf("second child process id is: %d\n", getpid());
            char* message = "COMP 8567\n";
            write(textFile, message, strlen(message));

            int i;
            for (i = 0; i < 15; i++) 
            {
                printf("\nchild Process: process id = %d, parent process id = %d\n", getpid(), getppid());
                sleep(1);
            }

            num1 += 10;
            num2 += 10;
            num3 += 10;

            printf("\nchild process: num1 = %d, num2 = %d, num3 = %d\n", num1, num2, num3);

            exit(0);
        }
        else if (process2 > 0)
        {
            int status;
            wait(&status);
            if (WIFEXITED(status)) 
            {
                printf("process ended normally with status-> %d\n",WEXITSTATUS(status));
            } 
            else if (WIFSIGNALED(status)) 
            {
                printf("process ended abnormally by killed signal: %d\n",WTERMSIG(status));
            }
                        
            printf("\nprocess id of main process is: %d\n", getpid());
            char* message = "HELLO FROM PARENT\n";
            write(textFile, message, strlen(message));

            num1 += 25;
            num2 += 25;
            num3 += 25;

            printf("\nparent Process: num1 = %d, num2 = %d, num3 = %d\n", num1, num2, num3);

            exit(0);
        }
        else
        {
            printf("\n!!Error Ocuured in Fork!!\n");
        }
    }
    else
    {
        printf("\n!!Error Ocuured in Fork!!\n");
    }
    exit(0);
}