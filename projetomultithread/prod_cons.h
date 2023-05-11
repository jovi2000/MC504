#ifndef PROD_CONS_H
#define PROD_CONS_H

/***
 * Opções disponíveis no cardápio
 * Cada comida pode ter um tempo diferente de preparação e consumo, assim o restaurante fica bem "paralelizado"
 * */

enum Food {
    Frango,
    Carbonara,
    Porpeta,
    Mignon,
    Camarao
};

/**
 * Estrutura da lista que será a fila
 * Cliente é uma senha enquanto está na fila, e ao entra no restaurante, ele vira uma Thread cujo ID é 'senha'.
 * */
typedef struct Cliente {

    struct Cliente *next;  //aponta pro próximo da fila

    long *senha; // ID do cliente
    enum Food food; //tipo de comida que o Cliente pode pedir(ele já sabe com antecedência)
} Cliente;

typedef struct Cliente *p_no;

typedef struct Fila {
    p_no start;
    p_no last;
} Fila;

typedef struct ArgsMesa {
    long id;
    p_no start;
} ArgsMesa;

// p_no *start; // ponteiro pro início da fila
// p_no *last; // ponteiro pro final da fila

/***
 * Coloca mais um cliente na fila pra entrar no restaurante.
 * Atualiza o ponteiro de fim
*/
void* colocar_clientes_fila(void* args);



#endif