//Rohit Kumar(110088741)
//Section:3
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


// created global-varibales
pid_t child1, child2, child3;
int setGrandChildKillFlag = 0;
int setGrandChildPauseFlag = 0;

// below-method-sets-the-flag-for-GrandChild
void setGrandChildSigFlag(int sigFlag)
{
    setGrandChildPauseFlag = 1;
}

// below-method-prints-the-grandchild-process
void invokeGrandChildSigHandler(int sigHandler)
{
    printf("Terminating GrandChild Process: PID: %d || PPID %d\n ", getpid(), getppid());
    exit(0);
}

// below-method-kill-the-grandchild
void invokeGrandChildTerminationHandler(int sigTermination)
{
    if (sigTermination == SIGINT)
    {
        if (setGrandChildKillFlag == 0)
        {
            // terminate grand children using pgid.
            killpg(getpid(), SIGUSR1);
            kill(child1, SIGUSR1);
            kill(child2, SIGUSR1);
            kill(child3, SIGUSR1);
            setGrandChildKillFlag = 1;
        }
    }
}

int main()
{
    int mainProcessId = getpid();

    child1 = fork();
    child2 = fork();
    child3 = fork();

    // checks-if-error-in-fork
    if (child1 < 0 || child2 < 0 || child3 < 0)
    {
        printf("!!!Error Occured: In forking()!!!");
        exit(1);
    }

    // checks-if-it-is-child
    if (getpid() != mainProcessId)
    {

        // ignore ctrl + C in children
        signal(SIGINT, SIG_IGN);

        // SIGUSR1 for interprocess communication
        signal(SIGUSR1, invokeGrandChildSigHandler);

        // group as of main process., remove immediate childs from that process group.
        if (getppid() == mainProcessId)
        {
            signal(SIGUSR1, setGrandChildSigFlag);

            if (setpgid(getpid(), getpid()) == -1)
            {
                printf("!!!Error Occured: In SetPGID()!!!");
                exit(1);
            }
        }
        for (;;)
        {
            printf("This is from process:%d\n", getpid());
            if (setGrandChildPauseFlag == 0)
                sleep(2);
            else
                sleep(10);
        }
    }
    else
    {
        // execution-of-parent-process
        signal(SIGINT, invokeGrandChildTerminationHandler);

        // ignore-SIGUSR1
        signal(SIGUSR1, SIG_IGN);

        // ignore-SIGCONT
        signal(SIGCONT, SIG_IGN);

        sleep(2);

        for (;;)
        {
            sleep(2);
            if (setGrandChildKillFlag != 0)
            {
                // below-logic-will-terminate-grandchildren
                printf("This is from process %d\n", getpid());

                kill(child1, SIGCONT);
                kill(child2, SIGCONT);
                kill(child3, SIGCONT);
            }
        }
    }
    return 0;
}