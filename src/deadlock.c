
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include "deadlock.h"

typedef struct
{
    pthread_mutex_t *mutex_a;
    pthread_mutex_t *mutex_b;
    pthread_mutex_t *first;
    pthread_mutex_t *second;
    pthread_barrier_t *barrier;
    int safe_mode;
    int id;
} DeadlockArgs;

static void *deadlock_worker(void *arg)
{
    DeadlockArgs *data = (DeadlockArgs *)arg;

    if (data->safe_mode)
    {
        /* Prevention: always lock A before B. */
        pthread_mutex_lock(data->mutex_a);
        printf("[Safe Thread %d] Locked mutex A\n", data->id);

        pthread_mutex_lock(data->mutex_b);
        printf("[Safe Thread %d] Locked mutex B\n", data->id);

        printf("[Safe Thread %d] Work completed safely\n", data->id);

        pthread_mutex_unlock(data->mutex_b);
        pthread_mutex_unlock(data->mutex_a);
    }
    else
    {
        /* Demonstrate opposite lock ordering without hanging. */
        pthread_mutex_lock(data->first);
        printf("[Demo Thread %d] Locked its first mutex\n", data->id);

        pthread_barrier_wait(data->barrier);

        int result = pthread_mutex_trylock(data->second);

        if (result == 0)
        {
            printf("[Demo Thread %d] Acquired second mutex\n",
                   data->id);
            pthread_mutex_unlock(data->second);
        }
        else if (result == EBUSY)
        {
            printf("[Demo Thread %d] Second mutex is busy: "
                   "deadlock risk detected\n", data->id);
        }
        else
        {
            printf("[Demo Thread %d] Mutex operation failed\n",
                   data->id);
        }

        pthread_mutex_unlock(data->first);
    }

    return NULL;
}

void run_deadlock_demo(void)
{
    pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;
    pthread_barrier_t barrier;

    pthread_t t1, t2;

    DeadlockArgs a1 = {
        &mutex_a, &mutex_b, &mutex_a, &mutex_b,
        &barrier, 0, 1
    };

    DeadlockArgs a2 = {
        &mutex_a, &mutex_b, &mutex_b, &mutex_a,
        &barrier, 0, 2
    };

    printf("\n--- Deadlock Risk Demonstration ---\n");

    if (pthread_barrier_init(&barrier, NULL, 2) != 0)
    {
        printf("Could not initialize synchronization barrier.\n");
        pthread_mutex_destroy(&mutex_a);
        pthread_mutex_destroy(&mutex_b);
        return;
    }

    int r1 = pthread_create(&t1, NULL, deadlock_worker, &a1);
    int r2 = pthread_create(&t2, NULL, deadlock_worker, &a2);

    if (r1 == 0 && r2 == 0)
    {
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
    }
    else
    {
        printf("Could not create both demo threads.\n");

        if (r1 == 0)
        {
            /* Release a possible thread waiting at the barrier. */
            pthread_barrier_wait(&barrier);
            pthread_join(t1, NULL);
        }

        if (r2 == 0)
        {
            pthread_barrier_wait(&barrier);
            pthread_join(t2, NULL);
        }
    }

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex_a);
    pthread_mutex_destroy(&mutex_b);

    printf("Deadlock risk demonstration finished.\n\n");
}

void run_deadlock_prevention_demo(void)
{
    pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;

    pthread_t t1, t2;

    DeadlockArgs a1 = {
        &mutex_a, &mutex_b, NULL, NULL, NULL, 1, 1
    };

    DeadlockArgs a2 = {
        &mutex_a, &mutex_b, NULL, NULL, NULL, 1, 2
    };

    printf("\n--- Deadlock Prevention Demonstration ---\n");
    printf("Rule: acquire mutex A before mutex B.\n");

    int r1 = pthread_create(&t1, NULL, deadlock_worker, &a1);
    int r2 = pthread_create(&t2, NULL, deadlock_worker, &a2);

    if (r1 == 0)
        pthread_join(t1, NULL);

    if (r2 == 0)
        pthread_join(t2, NULL);

    if (r1 != 0 || r2 != 0)
        printf("Warning: one or more threads could not be created.\n");

    pthread_mutex_destroy(&mutex_a);
    pthread_mutex_destroy(&mutex_b);

    printf("Deadlock prevention demonstration finished.\n\n");
}
