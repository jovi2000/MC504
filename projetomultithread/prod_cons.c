#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include "prod_cons.h" 

#define NUM_THREADS 5
#define NUM_MESAS 4
#define NUM_CHEFFS 1 

// int mesas_ocupadas = 0; //talvez fazer isso ser um semáforo?

sem_t semPedidosFeitos;
sem_t semPedidosProntos;
sem_t semFila;

pthread_mutex_t mutexBuffer;
pthread_mutex_t mutexFila;

char* buffer[NUM_MESAS]; // buffer de comida com o mesmo tamanho do numero de mesas, já q n faz sentido ser maior. Talvez fazer o buffer ter o tamanho de NUM_CHEFFS
int count = 0; // numero atual de comidas no buffer

/**
 * Chef que produz uma comida
 * */ 
void* chef(void* args) {
    while (1) {
        // Espera o cliente fazer o pedido
        sem_wait(&semPedidosFeitos);
    
        // Produce
        char* x = "Food";
        sleep(1); // random

        // Adicona no buffer
        pthread_mutex_lock(&mutexBuffer);
        buffer[count] = x;
        count++;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semPedidosProntos);
    }
}

/** Mesa recebe um cliente e depois dele comer imprime o proprio ID. 
 * Talvez fazer esse ID ser uma "senha" que se obtem na fila, como se fosse uma senha pra obter a comida especifica.
 * */
void* mesa(void* args) {
    while (1) {
        char* comida;
        long idMesa = (long)args;
        long idCliente;

        sem_wait(&semFila);
        pthread_mutex_lock(&mutexFila);
        //TODO(): pegar o primeiro cliente(com seu id) da fila e colocar na mesa
        //idCLiente = fila.senha
        pthread_mutex_unlock(&mutexFila);

        // Remove uma comida do buffer
        sem_post(&semPedidosFeitos);
        sem_wait(&semPedidosProntos);
        pthread_mutex_lock(&mutexBuffer);
        comida = buffer[count - 1];
        count--;
        pthread_mutex_unlock(&mutexBuffer);

        // Consome a comida
        printf("%ld got %s\n", idMesa ,comida);
        sleep(1); //random

        // Sai da mesa TODO()
    }
}

void* fila(void* args) {
    while (1) {
        long i = 0;
        sleep(1);
        //adiciona cliente i na fila
        i++;
        sem_post(&semFila);
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    //quando for fazer a fila, precisa fazer algum sistema pra ver qual mesa(th[id]) tá disponivel pra passar como argumento de pthread_create
    pthread_t th[NUM_THREADS]; 
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semPedidosFeitos, 0, NUM_MESAS);
    sem_init(&semPedidosProntos, 0, 0);
    sem_init(&semPedidosProntos, 0, 100);
    long i;
    for (i = 0; i < NUM_THREADS; i++) {
        if (i < 2) {
            if (pthread_create(&th[i], NULL, chef, (void *)i) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, mesa, (void *)i) != 0) {
                perror("Failed to create thread");
            }
        }
    }
    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semPedidosFeitos);
    sem_destroy(&semPedidosProntos);
    pthread_mutex_destroy(&mutexBuffer);
    return 0;
}