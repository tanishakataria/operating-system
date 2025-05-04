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

// Generate a random lowercase alphabet character
char generateRandomChar() {
    return (char)('a' + rand() % 26);
}

// Reader thread function
void *reader(void *arg) {
    while (1) {
        sem_wait(&mutex);
        readers_count++;
        if (readers_count == 1) {
            sem_wait(&rw_mutex); // First reader locks writers out
        }
        sem_post(&mutex);

        // Reading from the file
        fseek(file, 0, SEEK_SET);
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("Reader %ld: %s", (long)arg, buffer);
        }

        sem_wait(&mutex);
        readers_count--;
        if (readers_count == 0) {
            sem_post(&rw_mutex); // Last reader allows writers
        }
        sem_post(&mutex);

        usleep(1000); // Small delay
    }
    return NULL;
}

// Writer thread function
void *writer(void *arg) {
    while (1) {
        sem_wait(&rw_mutex); // Ensure exclusive access for writers

        // Generate random string
        srand(time(NULL));
        char randomString[STRING_LENGTH + 1];
        for (int i = 0; i < STRING_LENGTH; i++) {
            randomString[i] = generateRandomChar();
        }
        randomString[STRING_LENGTH] = '\0';

        // Append to file
        fseek(file, 0, SEEK_END);
        fprintf(file, "%s\n", randomString);
        printf("Writer %ld: %s\n", (long)arg, randomString);
        fflush(file);

        sem_post(&rw_mutex); // Release access

        usleep(1000000 + rand() % 2000000); // Delay for 1–3 seconds
    }
    return NULL;
}

int main() {
    file = fopen("shared_file.txt", "a+");
    if (file == NULL) {
        perror("Unable to open file");
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

    // Wait for threads to finish
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    printf("All threads have terminated.\n");

    sem_destroy(&mutex);
    sem_destroy(&rw_mutex);
    fclose(file);

    return 0;
}
