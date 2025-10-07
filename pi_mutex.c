#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int thread_count;
long long n;
double sum = 0.0;
pthread_mutex_t mutex;

double f(double x) { return 4.0 / (1.0 + x * x); }

void* Thread_sum(void* rank) {
    long my_rank = (long)rank;
    double my_sum = 0.0;
    double h = 1.0 / (double)n;
    long long i;

    for (i = my_rank; i < n; i += thread_count) {
        double x = h * (i + 0.5);
        my_sum += f(x);
    }

    pthread_mutex_lock(&mutex);
    sum += my_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[]) {
    long thread;
    struct timeval start, end;

    if (argc != 3) {
        printf("Uso: %s <n> <num_threads>\n", argv[0]);
        return 1;
    }

    n = atoll(argv[1]);
    thread_count = atoi(argv[2]);
    pthread_t thread_handles[thread_count];

    pthread_mutex_init(&mutex, NULL);

    gettimeofday(&start, NULL);

    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Thread_sum, (void*)thread);

    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_usec - start.tv_usec) / 1e6;

    double pi_est = (1.0 / (double)n) * sum;
    printf("pi (mutex) = %.15f\n", pi_est);
    printf("Tiempo de ejecucion: %.6f segundos\n", elapsed);

    pthread_mutex_destroy(&mutex);
    return 0;
}
