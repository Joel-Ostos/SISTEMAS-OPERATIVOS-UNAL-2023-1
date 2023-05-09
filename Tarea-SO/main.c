#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_HILOS 4 

typedef struct {
    long iteraciones;
    long inicio;
    double resultado;
} argumento_t;

void *leibniz(void *arg){
    argumento_t *argumentos = (argumento_t *)arg;
    long iteraciones = argumentos->iteraciones;
    long inicio = argumentos->inicio;
    double num = 4, calcular = 0;
    long n = 1 + 2 * inicio;
    int signo = (inicio % 2 == 0) ? 1 : -1;
    for (long i = 0; i < iteraciones; i++){
        calcular += (num / n) * signo;
        signo *= -1;
        n += 2;
    };
    argumentos->resultado = calcular;
}

int main(int argc, char *argv[]){
    pthread_t threads[NUM_HILOS];
    argumento_t argumentos[NUM_HILOS];
    long iteraciones = 10000000000;
    long iteraciones_por_hilo = iteraciones / NUM_HILOS;

    struct timespec inicio, fin;
    double tiempo_transcurrido;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < NUM_HILOS; i++) {
        argumentos[i].iteraciones = (i == NUM_HILOS - 1) ? (iteraciones - iteraciones_por_hilo * (NUM_HILOS - 1)) : iteraciones_por_hilo;
        argumentos[i].inicio = i * iteraciones_por_hilo;
        if (0 != pthread_create(&threads[i], NULL, leibniz, &argumentos[i]))
            return -1;
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(threads[i], NULL);
    }

    double resultado = 0;
    for (int i = 0; i < NUM_HILOS; i++) {
        resultado += argumentos[i].resultado;
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);
    tiempo_transcurrido = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("Numero de hilos: %d\n", NUM_HILOS);
    printf("Numero de iteraciones: %ld\n", iteraciones);
    printf("Resultado: %1.20lf\n", resultado);
    printf("Tiempo transcurrido: %.6f segundos\n", tiempo_transcurrido);

    return 0;
}

