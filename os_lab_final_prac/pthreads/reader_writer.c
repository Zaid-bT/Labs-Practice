#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2
#define STRING_LENGTH 60

pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
sem_t mutex, rw_mutex;

int readers_count = 0;
FILE *file;

/* Generate random lowercase character */
char generateRandomChar() {
    return (char)('a' + rand() % 26);
}

/* ---------------- READER ---------------- */
void *reader(void *arg) {
    long id = (long)arg;

    while (1) {

        sem_wait(&mutex);
        readers_count++;

        if (readers_count == 1) {
            sem_wait(&rw_mutex); // first reader locks writer
        }

        sem_post(&mutex);

        // Reading from file
        fseek(file, 0, SEEK_SET);

        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("Reader %ld: %s", id, buffer);
        }

        sem_wait(&mutex);
        readers_count--;

        if (readers_count == 0) {
            sem_post(&rw_mutex); // last reader unlocks writer
        }

        sem_post(&mutex);

        usleep(1000);
    }

    return NULL;
}

/* ---------------- WRITER ---------------- */
void *writer(void *arg) {
    long id = (long)arg;

    while (1) {

        sem_wait(&rw_mutex);

        srand(time(NULL));

        char randomString[STRING_LENGTH + 1];

        for (int i = 0; i < STRING_LENGTH; i++) {
            randomString[i] = generateRandomChar();
        }

        randomString[STRING_LENGTH] = '\0';

        // Writing to file
        fseek(file, 0, SEEK_END);
        fprintf(file, "%s\n", randomString);

        printf("Writer %ld: %s\n", id, randomString);

        fflush(file);

        sem_post(&rw_mutex);

        usleep(1000);
    }

    return NULL;
}

/* ---------------- MAIN ---------------- */
int main() {

    file = fopen("shared_file.txt", "a+");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    sem_init(&mutex, 0, 1);
    sem_init(&rw_mutex, 0, 1);

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_create(&writers[i], NULL, writer, (void *)(long)i);
    }

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_create(&readers[i], NULL, reader, (void *)(long)i);
    }

    // Join threads (infinite loop so never ends)
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    printf("Reader pthread join completed\n");

    sem_destroy(&mutex);
    sem_destroy(&rw_mutex);

    fclose(file);

    return 0;
}