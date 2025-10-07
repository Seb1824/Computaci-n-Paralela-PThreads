#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

int main(int argc, char* argv[]) {
    long long n;
    double h, x, sum = 0.0;
    struct timeval start, end;

    if (argc != 2) {
        printf("Uso: %s <n>\n", argv[0]);
        return 1;
    }

    n = atoll(argv[1]);
    h = 1.0 / (double)n;

    gettimeofday(&start, NULL);

    for (long long i = 0; i < n; i++) {
        x = h * (i + 0.5);
        sum += f(x);
    }

    double pi_est = h * sum;

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec)+(end.tv_usec - start.tv_usec) / 1e6;

    printf("pi (secuencial) = %.15f\n", pi_est);
    printf("Tiempo de ejecucion: %.6f segundos\n", elapsed);
    return 0;
}
