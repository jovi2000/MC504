#ifndef RESTAURANTE_H
#define RESTAURANTE_H

/**
 * Estrutura do cliente que pertence, inicialmente, a fila
 * */
typedef struct Cliente {

    struct Cliente *next;  // Aponta pro próximo da fila

    long *senha; // ID do cliente
} Cliente;

typedef struct Cliente *p_no;

typedef struct Fila {
    p_no start;
    p_no last;
} Fila;

typedef struct EstadoMesas {
    long idCliente;
    char *comida;
} EstadoMesas;

typedef struct ArgsMesa {
    long id;
    Fila *fila;
    EstadoMesas *estado;
} ArgsMesa;

/***
 * Coloca mais um cliente na fila pra entrar no restaurante.
 * Atualiza o ponteiro de fim
*/
void* colocar_clientes_fila(void* args);

/**
 * Função thread do Chef que produz as comidas e coloca no buffer conforme os
 * pedidos dos clientes vão chegando através de um signal
 * */ 
void* chef(void* args);

/** Função thread da Mesa que pega o cliente que está no começo da fila e acompanha
 * o fluxo do cliente, que no caso é pegar comida do buffer, comer e, por fim, 
 * sair da mesa
 * */
void* mesa(void* args);

/** Printa o estado atual que o restaurante se encontra, no caso, o estado da fila,
 * das mesas e do buffer
 * */
void print_estado_restaurante(Fila *fila, EstadoMesas *estadoMesa);



#endif