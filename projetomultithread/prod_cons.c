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

sem_t semPedidosFeitos;
sem_t semPedidosProntos;
sem_t semFila;

pthread_mutex_t mutexBuffer;
pthread_mutex_t mutexFila;
pthread_mutex_t mutexPrint;


char* buffer[NUM_MESAS]; // buffer de comida com o mesmo tamanho do numero de mesas, já q n faz sentido ser maior. Talvez fazer o buffer ter o tamanho de NUM_CHEFFS
int count = 0; // numero atual de comidas no buffer

/***
 * Opções disponíveis no cardápio
 * Cada comida pode ter um tempo diferente de preparação e consumo, assim o restaurante fica bem "paralelizado"
 * */

char *food[5] = {"Frango   ", "Carbonara", "Porpeta  ", "Mignon   ", "Camarão  "};


void print_estado_restaurante(Fila *fila, EstadoMesas *estadoMesa) {
  int largura=9;  
  printf("                    |---------------------------------------------------------|\n");
  printf("                    |                                                         |\n");
  printf("                    |   |---------------------------------------| |----|      |\n");
  if (count == 0) {
  printf("                    |   |%-*s|%-*s|%-*s|%-*s| |Chef|      |\n",
  largura,"",largura,"",largura,"",largura,"");
  } 
  else if (count == 1) {
  printf("                    |   |%-*s|%-*s|%-*s|%-*s| |Chef|      |\n",
  largura,buffer[0],largura,"",largura,"",largura,"");
  } 
  else if (count == 2) {
  printf("                    |   |%-*s|%-*s|%-*s|%-*s| |Chef|      |\n",
  largura,buffer[0],largura,buffer[1],largura,"",largura,"");
  } 
  else if (count == 3) {
  printf("                    |   |%-*s|%-*s|%-*s|%-*s| |Chef|      |\n",
  largura,buffer[0],largura,buffer[1],largura,buffer[2],largura,"");
  } 
  else if (count == 4) {
  printf("                    |   |%-*s|%-*s|%-*s|%-*s| |Chef|      |\n",
  largura,buffer[0],largura,buffer[1],largura,buffer[2],largura,buffer[3]);
  }
  printf("                    |   |---------------------------------------| |----|      |\n");
  printf("                    |                                                         |\n");
  printf("                    |                                                         |\n");
  if (estadoMesa[0].comida == NULL && estadoMesa[1].comida == NULL) {
  printf("                    |                                                         |\n");
  } 
  else if (estadoMesa[0].comida != NULL && estadoMesa[1].comida == NULL) {
  printf("                    |         %-*s                                       |\n", largura,estadoMesa[0].comida);
  } 
  else if (estadoMesa[0].comida == NULL && estadoMesa[1].comida != NULL) {
  printf("                    |         %-*s                                       |\n", largura,estadoMesa[1].comida);
  }
  else if (estadoMesa[0].comida != NULL && estadoMesa[1].comida != NULL) {
  printf("                    |         %-*s                        %-*s      |\n", largura,estadoMesa[0].comida, largura,estadoMesa[1].comida);
  }
  if (estadoMesa[0].idCliente == -1 && estadoMesa[1].idCliente == -1) {
  printf("                    |      |\\`====='/|                     |\\`====='/|        |\n");
  } 
  else if (estadoMesa[0].idCliente != -1 && estadoMesa[1].idCliente == -1) {
  printf("                    | Cl%3ld|\\`====='/|                     |\\`====='/|        |\n", estadoMesa[0].idCliente);
  }
  else if (estadoMesa[0].idCliente == -1 && estadoMesa[1].idCliente != -1) {
  printf("                    |      |\\`====='/|                Cl%3ld|\\`====='/|        |\n", estadoMesa[1].idCliente);
  } 
  else if (estadoMesa[0].idCliente != -1 && estadoMesa[1].idCliente != -1) {
  printf("                    | Cl%3ld|\\`====='/|                Cl%3ld|\\`====='/|        |\n", estadoMesa[0].idCliente,estadoMesa[1].idCliente);
  }
  printf("                    |      |  Mesa0  |                     |  Mesa1  |        |\n");
  printf("                    |                                                         |\n");
  printf("--------------------                                                          |\n");

  if (fila->start==NULL){
  printf("                                                                              |\n");
  }
  else if (fila->start!=NULL && fila->start->next==NULL){
  printf("               Cl%3ld                                                          |\n",*fila->start->senha);
  }
  else if (fila->start!=NULL && fila->start->next!=NULL && fila->start->next->next==NULL ){
  printf("     Cl%3ld      Cl%3ld                                                         |\n",*fila->start->next->senha,*fila->start->senha);
  }
  else if (fila->start!=NULL && fila->start->next!=NULL && fila->start->next->next!=NULL 
  && fila->start->next->next->next==NULL ){
  printf("     Cl%3ld Cl%3ld Cl%3ld                                                        |\n",*fila->start->next->next->senha,*fila->start->next->senha,*fila->start->senha);
  }
  else if (fila->start!=NULL && fila->start->next!=NULL && fila->start->next->next!=NULL 
  && fila->start->next->next->next!=NULL){
  printf("Cl%3ld Cl%3ld Cl%3ld Cl%3ld                                                       |\n",*fila->start->next->next->next->senha,*fila->start->next->next->senha,*fila->start->next->senha,*fila->start->senha);
  }

  
  printf("--------------------                                                          |\n");

  if (estadoMesa[2].comida == NULL && estadoMesa[3].comida == NULL) {
  printf("                    |                                                         |\n");
  } 
  else if (estadoMesa[2].comida != NULL && estadoMesa[3].comida == NULL) {
  printf("                    |         %-*s                                       |\n", largura,estadoMesa[2].comida);
  }
  else if (estadoMesa[2].comida == NULL && estadoMesa[3].comida != NULL) {
  printf("                    |         %-*s                                       |\n", largura,estadoMesa[3].comida);
  } 
  else if (estadoMesa[2].comida != NULL && estadoMesa[3].comida != NULL) {
  printf("                    |         %-*s                       %-*s       |\n", largura,estadoMesa[2].comida, largura,estadoMesa[3].comida);
  }
  if (estadoMesa[2].idCliente == -1 && estadoMesa[3].idCliente == -1) {
  printf("                    |      |\\`====='/|                     |\\`====='/|        |\n");
  } 
  else if (estadoMesa[2].idCliente != -1 && estadoMesa[3].idCliente == -1) {
  printf("                    | Cl%3ld|\\`====='/|                     |\\`====='/|        |\n", estadoMesa[2].idCliente);
  } 
  else if (estadoMesa[2].idCliente == -1 && estadoMesa[3].idCliente != -1) {
  printf("                    |      |\\`====='/|                Cl%3ld|\\`====='/|        |\n", estadoMesa[3].idCliente);
  }
  else if (estadoMesa[2].idCliente != -1 && estadoMesa[3].idCliente != -1) {
  printf("                    | Cl%3ld|\\`====='/|               Cl %3ld|\\`====='/|        |\n", estadoMesa[2].idCliente,estadoMesa[3].idCliente);
  }
  printf("                    |      |  Mesa2  |                     |  Mesa3  |        |\n");
  printf("                    |                                                         |\n");
  printf("                    |---------------------------------------------------------|\n");
  printf("\n");
}

/**
 * Chef que produz uma comida
 * */ 
void* chef(void* args) {
    while (1) {
        // Espera o cliente fazer o pedido
        sem_wait(&semPedidosFeitos);
    
        // Cozinhando
        char* comida = food[rand()%5];
        sleep((rand()%1));

        // Adicona comida no buffer
        pthread_mutex_lock(&mutexBuffer);
        //printf("Cozinheiro coloca %s no buffer\n", comida);
        buffer[count] = comida;
        count++;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semPedidosProntos);
    }
}

/** Mesa recebe um cliente e depois dele comer imprime o proprio ID. 
 * Talvez fazer esse ID ser uma "senha" que se obtem na fila, como se fosse uma senha pra obter a comida especifica.
 * */
void* mesa(void* args) {
    ArgsMesa *argsMesa = (ArgsMesa *) args;
    while (1) {
      char* comida;
      long idMesa = argsMesa->id;

      // Cliente pega seu lugar na mesa  
      sem_wait(&semFila);
      pthread_mutex_lock(&mutexFila);
      Cliente clienteAtual = *argsMesa->fila->start;
      p_no aux = argsMesa->fila->start;
      argsMesa->fila->start = argsMesa->fila->start->next;
      argsMesa->estado[idMesa].idCliente = *clienteAtual.senha;
      pthread_mutex_lock(&mutexPrint);
      print_estado_restaurante(argsMesa->fila, argsMesa->estado);
      pthread_mutex_unlock(&mutexPrint);
      //printf("Cliente %ld senta na mesa %ld\n", *clienteAtual.senha, idMesa);
      pthread_mutex_unlock(&mutexFila);

      // Faz o pedido
      sem_post(&semPedidosFeitos);
      sem_wait(&semPedidosProntos);
      // Remove uma comida do buffer
      sleep((rand()%3)+1); // Isso impede que o cozinheiro fique travado no mutex do buffer, dessa forma ele consegue colocar mais comidas no buffer
      pthread_mutex_lock(&mutexBuffer);
     // printf("buffer %ld\n", *clienteAtual.senha);
     // printf("Cliente %ld começa a comer\n", *clienteAtual.senha);
      sleep((rand()%4)+2); // Tempo do cliente ir até o buffer e pegar a comida
      comida = buffer[count - 1];
      argsMesa->estado[idMesa].comida = buffer[count - 1];
      count--;
      pthread_mutex_lock(&mutexPrint);
      print_estado_restaurante(argsMesa->fila, argsMesa->estado);
      pthread_mutex_unlock(&mutexPrint);
      pthread_mutex_unlock(&mutexBuffer);

      // Consome a comida
      sleep((rand()%8)+1);
      //printf("Cliente %ld da mesa %ld terminou de comer %s\n", *clienteAtual.senha, idMesa ,comida);
  
      // Sai da mesa
      argsMesa->estado[idMesa].idCliente = -1;
      argsMesa->estado[idMesa].comida = NULL;
      free(aux->senha);
      free(aux);
    }
}

void* colocar_clientes_fila(void* args) {
    long i = 0;
    while (1) {
      Fila *fila = (Fila *) args;
      sleep((rand()%3)+2);

      //Cria cliente
      p_no novoCliente;
      novoCliente = malloc(sizeof(Cliente));
      novoCliente->senha = malloc(sizeof(long));
      *novoCliente->senha = i;
      novoCliente->next = NULL;

      //Coloca cliente na fila
      if (fila->start == NULL) {
        pthread_mutex_lock(&mutexFila);
        fila->start = novoCliente;
        fila->last = novoCliente;
       // printf("Cliente %ld colocado na fila\n", *fila->start->senha);
        pthread_mutex_unlock(&mutexFila);
      }
      else {
        pthread_mutex_lock(&mutexFila);
        fila->last->next = novoCliente;
        fila->last = fila->last->next;
        //printf("Cliente %ld colocado na fila\n", *fila->last->senha);
        
        pthread_mutex_unlock(&mutexFila);
      }
      sem_post(&semFila);
      i++;
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    //quando for fazer a fila, precisa fazer algum sistema pra ver qual mesa(th[id]) tá disponivel pra passar como argumento de pthread_create
    pthread_t th[NUM_THREADS]; 
    pthread_mutex_init(&mutexFila, NULL);
    pthread_mutex_init(&mutexBuffer, NULL);
    pthread_mutex_init(&mutexPrint, NULL);
    sem_init(&semPedidosFeitos, 0, 0);
    sem_init(&semPedidosProntos, 0, 0);
    sem_init(&semFila, 0, 0);
    long i;

    // Cria fila:
    Fila *fila;
    fila = malloc(sizeof(Fila));
    fila->start = NULL;
    fila->last = NULL;

    // Vetor estado das mesas:
    EstadoMesas estadoMesas[4];

    // Argumentos da mesa:
    ArgsMesa vectorArgsMesa[NUM_MESAS];
    for (long j = 0; j < NUM_MESAS; j++) {
      vectorArgsMesa[j].fila = fila;
      vectorArgsMesa[j].id = j;
      estadoMesas[j].idCliente = -1;
      estadoMesas[j].comida = NULL;
      vectorArgsMesa[j].estado = estadoMesas;
    }

    // Cria 1 Thread que coloca os clientes que chegam na fila
    if (pthread_create(&th[0], NULL, colocar_clientes_fila, fila) != 0) {
      perror("Failed to create thread");
    }

    // Cria 1 Thread para o Chef e 4 Threads para as Mesas
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

    // Faz o join das Threads
    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    // Apaga as Threads e a Fila
    sem_destroy(&semPedidosFeitos);
    sem_destroy(&semPedidosProntos);
    sem_destroy(&semFila);
    pthread_mutex_destroy(&mutexBuffer);
    free(fila);
    return 0;
}