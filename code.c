#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MAX 1000000

long long N;
int P;

long long factors[MAX];
int count = 0;

pthread_mutex_t lock;

typedef struct {
    long long start;
    long long end;
} ThreadData;

void* find_factors(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    for (long long i = data->start; i <= data->end; i++) {
        if (N % i == 0) {

            /* Add i (if not N itself) */
            if (i != N) {
                pthread_mutex_lock(&lock);
                if (count < MAX)
                    factors[count++] = i;
                pthread_mutex_unlock(&lock);
            }

            long long pair = N / i;

            /* Add pair if it is distinct from i and not N itself */
            if (pair != i && pair != N) {
                pthread_mutex_lock(&lock);
                if (count < MAX)
                    factors[count++] = pair;
                pthread_mutex_unlock(&lock);
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Usage: %s <N> <P>\n", argv[0]);
        return 1;
    }

    N = atoll(argv[1]);
    P = atoi(argv[2]);

    if (N <= 1 || P <= 0) {
        printf("Invalid input\n");
        return 1;
    }

    long long limit = (long long)sqrt((double)N);

    /* Clamp P so we never create more threads than there are numbers to check */
    if (P > limit)
        P = (int)limit;

    pthread_t* threads = malloc(P * sizeof(pthread_t));
    ThreadData* td     = malloc(P * sizeof(ThreadData));

    if (!threads || !td) {
        printf("Memory allocation failed\n");
        return 1;
    }

    pthread_mutex_init(&lock, NULL);

    /*
     * Evenly distribute [1 .. limit] across P threads using integer arithmetic.
     * Thread i covers [ i*limit/P + 1 .. (i+1)*limit/P ]
     * guaranteeing no gaps and no overlaps.
     */
    for (int i = 0; i < P; i++) {
        td[i].start = (long long)i * limit / P + 1;
        td[i].end   = (long long)(i + 1) * limit / P;

        pthread_create(&threads[i], NULL, find_factors, &td[i]);
    }

    /* Wait for all threads to finish */
    for (int i = 0; i < P; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Sum all collected factors */
    long long sum = 0;
    for (int i = 0; i < count; i++) {
        sum += factors[i];
    }

    /* Report result */
    if (sum == N)
        printf("%lld is a PERFECT number\n", N);
    else
        printf("%lld is NOT a perfect number\n", N);

    pthread_mutex_destroy(&lock);
    free(threads);
    free(td);

    return 0;
}
