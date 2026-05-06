#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5  // number of philosophers

sem_t forks[N];  // one semaphore per fork

void *philosopher(void *num) {
    int id = (long)num;

    while (1) {

        // Thinking
        printf("Philosopher %d is thinking\n", id);
        sleep(rand() % 3);

        // Pick up forks (left and right)
        sem_wait(&forks[id]);
        sem_wait(&forks[(id + 1) % N]);

        // Eating
        printf("Philosopher %d is eating\n", id);
        sleep(rand() % 2);

        // Put down forks
        sem_post(&forks[id]);
        sem_post(&forks[(id + 1) % N]);

        printf("Philosopher %d finished eating\n", id);
    }

    return NULL;
}

int main() {
    pthread_t philo[N];

    // Initialize forks
    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        pthread_create(&philo[i], NULL, philosopher, (void *)(long)i);
    }

    // Join threads
    for (int i = 0; i < N; i++) {
        pthread_join(philo[i], NULL);
    }

    // Destroy semaphores (never reached due to infinite loop)
    for (int i = 0; i < N; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}