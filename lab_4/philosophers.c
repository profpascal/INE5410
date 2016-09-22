#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define PHILOS 5
#define DELAY 5000
#define FOOD 100

void *philosopher (void *id);
void grab_fork (int, int, char *);
void down_forks (int, int);
int food_on_table ();

pthread_mutex_t forks[PHILOS];
pthread_t philo[PHILOS];
pthread_mutex_t food_lock;
int sleep_seconds = 0;

int main (int argc, char **argv)
{
    int i;
    int params[PHILOS];

    if (argc == 2)
        sleep_seconds = atoi (argv[1]);

    pthread_mutex_init (&food_lock, NULL);

    for (i = 0; i < PHILOS; i++)
        pthread_mutex_init (&forks[i], NULL);

    for (i = 0; i < PHILOS; i++) {
        params[i] = i;
        pthread_create (&philo[i], NULL, philosopher, (void *)&params[i]);
    }

    for (i = 0; i < PHILOS; i++)
        pthread_join (philo[i], NULL);

    return 0;
}

void *
philosopher (void *num)
{
    int id;
    int i, left_fork, right_fork, f;

    id = *((int *)num);
    printf ("Philosopher %d is done thinking and now ready to eat.\n", id);
    right_fork = id + 1;
    left_fork = id;

    /* Wrap around the forks. */
    if (right_fork == PHILOS)
        right_fork = 0;

    while (f = food_on_table ()) {

        /* Thanks to philosopher #1 who would like to take a nap
         * before picking up the forks, the other philosophers
         * may be able to eat their dishes and not deadlock.
         */
        if (id == 1)
            sleep (sleep_seconds);
        
        /*
         * The odds try to pick first the left fork.
         * The even, first the right.
         */
        if (id%2 == 0) {
            grab_fork (id, right_fork, "left");
            grab_fork (id, left_fork, "right ");
        } else {
            grab_fork (id, left_fork, "right ");
            grab_fork (id, right_fork, "left");
        }
        printf ("Philosopher %d: eating.\n", id);
        usleep (DELAY * (FOOD - f + 1));
        down_forks (right_fork, left_fork);
    }

    printf ("Philosopher %d is done eating.\n", id);
    return (NULL);
}

int
food_on_table ()
{
    static int food = FOOD;
    int myfood;

    pthread_mutex_lock (&food_lock);
    if (food > 0) {
        food--;
    }
    myfood = food;
    pthread_mutex_unlock (&food_lock);
    return myfood;
}

void
grab_fork (int phil, int c, char *hand)
{
    pthread_mutex_lock (&forks[c]);
    printf ("Philosopher %d: got %s fork %d\n", phil, hand, c);
}

void
down_forks (int c1, int c2)
{
    pthread_mutex_unlock (&forks[c1]);
    pthread_mutex_unlock (&forks[c2]);
}
