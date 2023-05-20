# Projeto Multithread - Restaurante

## Tema

O tema escolhido pelo grupo foi uma simulação do funcionamento de um **restaurante**. Esse restaurante possui **um chefe de cozinha**, um **buffer** para armazenar as comidas prontas, **4 mesas** e uma **fila** do lado de fora do restaurante para comportar **clientes** que estão esperando para entrar.

## Como funciona cada componente do restaurante?

- **Chefe de cozinha:** O chefe funciona como um produtor e segue algumas regras: ele só começa a cozinhar quando algum cliente está ocupando uma mesa e essa mesa está sem nenhuma comida, caso contrário o chefe dorme até algum cliente se sentar na mesa e realizar um pedido. Ao terminar de cozinhar um pedido, o chefe coloca esse pedido no buffer para que o cliente possa se servir. O chefe cozinha aleatoriamente um dos seguintes tipos de comida: Frango, Carbonara, Porpeta, Mignon e Camarão.

- **Cliente:** O cliente funciona como um consumido e segue algumas regras: ele incialemnte entra na fila do restaurante e quando chega sua vez é colocado em uma das mesas, ao sentar na mesa um sinal é enviado ao chefe avisando que o cliente fez seu pedido e o cliente dorme até que consiga pegar alguma comida no buffer, caso ele tente entrar no buffer mas outro cliente pegue uma comida antes dele, ele volta a dormir. Depois de adquirir sua comida, o cliente passa um tempo comendo e ao terminar libera seu lugar na mesa e sai do restaurante.

- **Mesa:**

- **Fila:**

- **Buffer:** O buffer serve para armazenar as comidas que estão prontas e são colocadas pelo chefe e para que os clientes possam se servir, uma vez que um cliente se serve essa comida é retirada do buffer. O buffer também segue algumas regras: somente o chefe e os clientes acessam o buffer e apenas 1 componente por vez pode acessar o buffer, caso um componente tente acessar o buffer enquanto ele estiver bloqueado (algum componente está acessando ele) esse componente precisará esperar. O tamanho máximo do buffer é 4, uma vez que o máximo de clientes que o restaurante pode ter é 4 e o chefe só cozinha 1 comida para cada, não podendo ultrapassar o número 4.

## Como funcionam as threads e o sistema de produto-consumidor usando mutex e semáforos do projeto?

## Como rodar o projeto?

- Primeiramente é necessário baixar a pasta "projetomultithread" em sua máquina;

- Após isso é só rodar os dois seguintes comandos em sequência no terminal:

```c
make

./restaurante.out
```

- Depois disso, os estados do restaurante vão começar a ser printados no terminal;

## O que o projeto printa?
