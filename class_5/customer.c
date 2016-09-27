#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX_THREADS 1

sem_t full;
sem_t empty;
sem_t produtor_semaphore;
sem_t consumidor_semaphore;

int f = 4;
int i = 0;


void *func_produtor() {
    printf("Thread produtora criada\n");
    int k = f;
    while (k > 0) {
        sem_wait(&empty);
        sem_wait(&produtor_semaphore);
        f = (f+1)%4;
        printf("Thread %u produziu f: %d\n", pthread_self(), f);
        sem_post(&produtor_semaphore);
        sem_post(&full);
        k -= 1;
    }
    pthread_exit(NULL);
}

void *func_consumidor() {
    printf("Thread consumidora criada\n");
    int k = i;
    while (k < 4) {
        sem_wait(&full);
        sem_wait(&consumidor_semaphore);
        i = (i+1)%4;
        printf("Thread %u consumiu i: %d\n", pthread_self(), i);
        sem_post(&consumidor_semaphore);
        sem_post(&empty);
        k += 1;
    }
    pthread_exit(NULL);
}


int main(int argc, char **argv) {
    int j;
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, 4);
    
    pthread_t produtor_threads[MAX_THREADS];
    pthread_t consumidor_threads[MAX_THREADS];
    sem_init(&produtor_semaphore, 0, 1);
    sem_init(&consumidor_semaphore, 0, 1);

    printf("Main thread iniciada.\n");
    for(j = 0; j < MAX_THREADS; j++)    // Criando threads produtoras
        pthread_create(&produtor_threads[j], NULL, func_produtor , NULL);

    for(j = 0; j < MAX_THREADS; j++)    // Criando threads consumidoras
        pthread_create(&consumidor_threads[j], NULL, func_consumidor, NULL);


    for(j = 0; j < MAX_THREADS; j++) {
        pthread_join(produtor_threads[j], NULL);
        pthread_join(consumidor_threads[j], NULL);
    }

    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&produtor_semaphore);
    sem_destroy(&consumidor_semaphore);
    return 0;
}

