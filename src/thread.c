#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *monitor(void *arg)
{
    (void)arg;

    while (1)
    {
        sleep(10);
        printf("\n[Monitor] DevSecOps Command Console Running...\n");
        fflush(stdout);
    }

    return NULL;
}

void start_monitor_thread(void)
{
    pthread_t tid;

    pthread_create(&tid, NULL, monitor, NULL);
    pthread_detach(tid);
}
