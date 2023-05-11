#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include "prod_cons.h" 

#define NUM_THREADS 6
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
        sleep(1); // random TODO()

        // Adicona no buffer
        pthread_mutex_lock(&mutexBuffer);
        printf("Cozinheiro coloca comida no buffer\n");
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
      ArgsMesa *argsMesa = (ArgsMesa *) args;
      char* comida;
      long idMesa = argsMesa->id;

      sem_wait(&semFila);
      pthread_mutex_lock(&mutexFila);
      printf("argsMesa: %p\n", argsMesa->start);
      printf("argsMesa: %p\n", argsMesa->start->senha);
      Cliente clienteAtual = *argsMesa->start;
      p_no aux = argsMesa->start;
      printf("aux: %p\n", aux);
      printf("%ld\n", *clienteAtual.senha);
      argsMesa->start = argsMesa->start->next;
      printf("next: %p\n", argsMesa->start->next);
      pthread_mutex_unlock(&mutexFila);

      // Remove uma comida do buffer
      sem_post(&semPedidosFeitos);
      sem_wait(&semPedidosProntos);
      printf("Cliente tenta entrar no buffer pra pegar comida\n");
      pthread_mutex_lock(&mutexBuffer);
      printf("Cliente entra no buffer pra pegar comida\n");
      comida = buffer[count - 1];
      count--;
      pthread_mutex_unlock(&mutexBuffer);

      // Consome a comida
      printf("Cliente %ld da mesa %ld comeu %s\n", *clienteAtual.senha, idMesa ,comida);
      sleep(1); //random

      // Sai da mesa TODO
    }
}

void* colocar_clientes_fila(void* args) {
    long i = 0;
    while (1) {
      Fila *fila = (Fila *) args;
      sleep(1); // random TODO()
      printf("dnv %p\n", fila->start);

      //Cria cliente
      p_no novoCliente;
      novoCliente = malloc(sizeof(Cliente));
      novoCliente->senha = malloc(sizeof(long));
      novoCliente->senha = &i;
      novoCliente->next = NULL;

      //Coloca cliente na fila
      if (fila->start == NULL) {
        pthread_mutex_lock(&mutexFila);
        fila->start = novoCliente;
        fila->last = novoCliente;
        pthread_mutex_unlock(&mutexFila);
      }
      else {
        pthread_mutex_lock(&mutexFila);
        fila->last->next = novoCliente; // SEGFAULT AQUI!!
        fila->last = fila->last->next;
        pthread_mutex_unlock(&mutexFila);
      }
      sem_post(&semFila);
      printf("Cliente %ld colocado na fila\n", *novoCliente->senha);
      i++;
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    //quando for fazer a fila, precisa fazer algum sistema pra ver qual mesa(th[id]) tá disponivel pra passar como argumento de pthread_create
    pthread_t th[NUM_THREADS]; 
    pthread_mutex_init(&mutexFila, NULL);
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semPedidosFeitos, 0, 0);
    sem_init(&semPedidosProntos, 0, 0);
    sem_init(&semFila, 0, 0);
    long i;

    // Cria fila:
    Fila *fila;
    fila = malloc(sizeof(Fila));
    fila->start = NULL;
    printf("endereço start fila: %p\n", &fila->start);
    fila->last = NULL;

    // Argumentos da mesa:
    ArgsMesa vectorArgsMesa[NUM_MESAS];
    for (long j = 0; j < NUM_MESAS; j++) {
      vectorArgsMesa[j].start = &fila->start;
      printf("endereço start fila2: %p\n", vectorArgsMesa[j].start);
      vectorArgsMesa[j].id = j;
    }

    if (pthread_create(&th[0], NULL, colocar_clientes_fila, fila) != 0) {
      perror("Failed to create thread");
    }

    for (i = 1; i < NUM_THREADS; i++) {
        if (i < 2) {
            if (pthread_create(&th[i], NULL, chef, (void *)i) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, mesa, &vectorArgsMesa[i-2]) != 0) {
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
    sem_destroy(&semFila);
    pthread_mutex_destroy(&mutexBuffer);
    free(fila);
    return 0;
}