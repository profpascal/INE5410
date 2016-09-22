#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SLEEP 10
#define MAX_THREADS 2

 pthread_mutex_t mutex1;
 pthread_mutex_t mutex2;

 void *thread_A (void *arg) {
     int random_time;

     while (1) {
         srand(time(NULL));
         random_time = rand() % MAX_SLEEP;
         printf("[%s]Sleeping for %d seconds\n", __FUNCTION__, random_time);
         sleep(random_time);
         printf("[%s]Trying to acquire mutex1 (holding none)\n", __FUNCTION__);
         pthread_mutex_lock(&mutex1);
         printf("[%s]Acquired mutex1\n", __FUNCTION__);

         random_time = rand() % MAX_SLEEP;
         printf("[%s]Sleeping for %d seconds\n", __FUNCTION__, random_time);
         sleep(random_time);

         printf("[%s]Trying to acquire mutex2 (holding mutex1) \n", __FUNCTION__);
         pthread_mutex_lock(&mutex2);
         printf("[%s]Acquired mutex2\n\n", __FUNCTION__);

         pthread_mutex_unlock(&mutex2);
         pthread_mutex_unlock(&mutex1);
     }
     return NULL;
 }

 void *thread_B (void *arg) {
     int random_time;

     while (1) {
         srand(time(NULL));
         random_time = rand() % MAX_SLEEP;
         printf("[%s]Sleeping for %d seconds\n", __FUNCTION__, random_time);
         sleep(random_time);
         printf("[%s]Trying to acquire mutex1 (holding none) \n", __FUNCTION__);
         pthread_mutex_lock(&mutex1);
         printf("[%s]Acquired mutex1\n\n", __FUNCTION__);

         random_time = rand() % MAX_SLEEP;
         printf("[%s]Sleeping for %d seconds\n", __FUNCTION__, random_time);
         sleep(random_time);
                  
         printf("[%s]Trying to acquire mutex2 (holding mutex1)\n", __FUNCTION__);
         pthread_mutex_lock(&mutex2);
         printf("[%s]Acquired mutex2\n", __FUNCTION__);

         pthread_mutex_unlock(&mutex2);
         pthread_mutex_unlock(&mutex1);
     }
     return NULL;
 }

 int main () {
   pthread_t threads[MAX_THREADS];
   int i;

   pthread_mutex_init(&mutex1, NULL);
   pthread_mutex_init(&mutex2, NULL);

   for(i=0; i < MAX_THREADS; i++) {
     if(i%2 == 0)
       pthread_create(&threads[i], NULL, thread_A, NULL);
     else
       pthread_create(&threads[i], NULL, thread_B, NULL);
   }

   for(i=0; i < MAX_THREADS; i++)
     pthread_join(threads[i], NULL);


    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
   return 0;
 }
