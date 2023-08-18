#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

int main()
{
    int child1 = fork();
    int child2 = fork();

    if(child2 < 0)
    {
        printf("Error in creating second child process\n");
        return 1;
    }
    else if (child2 ==0)
    {
        char *args[] = {"ls", "-1", "/home/kumar2d", NULL};
        execl("/bin/ls", "ls", "-1", "/home/kumar2d", (char *)NULL);   
    }
    else
    {
        //parent process
        printf("Main process completed and process id : %d\n", getpid());
    }
    

    if(child1 < 0)
    {
        printf("Error occured while creating first child process\n");
        return 1;
    }
    else if(child1 ==0)
    {
        // child 1 process
        int grandchild = fork();
        if (grandchild == 0)
        {
            // grandchild process
            printf("grandchild process and process id :%d\n", getpid());
            chdir("/home/kumar2d/lab4");

            int file_descriptor = open("sample.txt", O_CREAT | O_RDWR, 0777);

            if (file_descriptor == -1)
            {
                printf("Error occured while creating file");
                return 1;
            }

            printf("File Created successfully:\n");
        }
        else if (child1 == -1)
        {
            printf("Error while creating grand child");
            return 1;
        }
        else
        {
            // back to child 1 process
            printf("wait for grandchild process to complete\n");
            int status;
            int gcid = wait(&status);

            printf("Back to child 1 process: %d\n", getpid());

            char *args[] = {"ls", "-1", "/home/kumar2d/lab4", NULL};
            execv("/bin/ls", args);
        }
    }
    else
    {
        //parent process
        printf("Main process completed and process id : %d\n", getpid());
    }
}