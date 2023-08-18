// declare header files for the program
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


// declare global variables for the program
int incrementer = 6;
pthread_mutex_t mutexincrementerer;

// declare the thread Function for the program
void* threadFunc(void* arg) {
    int is_detachable = 0;
    pthread_t threadId = pthread_self();

// verify if created thread is in detachable mode
    if (pthread_detach(threadId) == 0) {
        is_detachable = 1;}

// decrement the mutex lock while holding
    pthread_mutex_lock(&mutexincrementerer);
    incrementer--; 
    if (is_detachable) {
        printf("Detached Thread ID: %lu, Incrementer: %d\n", threadId, incrementer);
        }         
    pthread_mutex_unlock(&mutexincrementerer);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[6];
    pthread_attr_t attr;
    int i, detachedState;

    // Initialize the mutex
    pthread_mutex_init(&mutexincrementerer, NULL);
    for (i = 0; i < 6; i++) 
    {
        pthread_attr_init(&attr);
        // Execute the detached for first three seconds and then execute the joinable 
        if (i < 3) 
            detachedState = PTHREAD_CREATE_JOINABLE;
        else 
            detachedState = PTHREAD_CREATE_DETACHED;

        pthread_attr_setdetachstate(&attr, detachedState);
        pthread_create(&threads[i], &attr, threadFunc, NULL);

        // sleep for 2 sec to ensure threads executes
        sleep(2);
    }

// wait for joinable threads to finish
    for (i = 0; i < 6; i++) {
        pthread_join(threads[i], NULL);
    }

// destroy mutex
    pthread_mutex_destroy(&mutexincrementerer);
    return 0;
}