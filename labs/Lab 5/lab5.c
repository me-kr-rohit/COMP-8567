#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

//declare variable globally
int resetCounter = 0;
time_t userInterruptionTime = 0;

//created customer SIGINThandler to catch the signal
void customSIGINTHandler(int signalNumber)
{
    //assigning the system-time to currentsystem-time
    time_t currentSystemTime = time(NULL);
    if (signalNumber == SIGINT)
    {
        if (currentSystemTime - userInterruptionTime <= 5)
        {
            exit(0);
        }
        else
        {
            //setting interrruption-time to current-time & increasing the resetcounter
            userInterruptionTime = currentSystemTime;
            resetCounter++;
        }
    }
}

//execution starts from main method
int main()
{
    //creating the signals
    signal(SIGINT, customSIGINTHandler);
    for(;;)
    {
        printf("Welcome to Lab5 - Signals\n");
        sleep(1);
    }
    return 0;
}
