# Projeto Multithread - Restaurante

## Tema

O tema escolhido pelo grupo foi uma simulação do funcionamento de um **restaurante**. Esse restaurante possui **um chefe de cozinha**, um **buffer** para armazenar as comidas prontas, **4 mesas** e uma **fila** do lado de fora do restaurante para comportar **clientes** que estão esperando para entrar.

## Como funciona cada componente do restaurante?

- **Chefe de cozinha:** O chefe funciona como uma thread produtora e segue algumas regras: ele só começa a cozinhar quando algum cliente está ocupando uma mesa e essa mesa está sem nenhuma comida, caso contrário o chefe dorme até algum cliente se sentar na mesa e realizar um pedido. Ao terminar de cozinhar um pedido, o chefe coloca esse pedido no buffer para que o cliente possa se servir. O chefe cozinha aleatoriamente um dos seguintes tipos de comida: Frango, Carbonara, Porpeta, Mignon e Camarão.

- **Cliente:** O cliente funciona como um consumidor e segue algumas regras: ele inicialmente entra na fila do restaurante e quando chega sua vez é colocado em uma das mesas, ao sentar na mesa um sinal é enviado ao chefe avisando que o cliente fez seu pedido e o cliente dorme até que consiga pegar alguma comida no buffer, caso ele tente entrar no buffer mas outro cliente pegue uma comida antes dele, ele volta a dormir. Depois de adquirir sua comida, o cliente passa um tempo comendo e ao terminar libera seu lugar na mesa e sai do restaurante.

- **Mesa:** A mesa funciona como uma thread consumidora, uma vez que acompanha todo o fluxo do cliente e qundo o cliente precisa pegar a comida do buffer quem na verdade faz isso é a própria mesa, uma vez que é essa quem tem acesso ao buffer, e segue algumas regras: se estiver vazia, a mesa busca o primeiro cliente da fila pra colocar nela. Após isso, a mesa envia um sinal pro Chef para que este comece a fazer a comida do cliente que está sentado nela. Ela só libera o cliente depois deste terminar de comer.

- **Fila:** A fila funciona como uma lista ligada que enfileira os clientes que chegam no restaurante. Cada novo cliente que chega vai para o final da fila. Se o restaurante estiver com alguma mesa disponível, o primeiro da fila entra no restaurante e senta nessa mesa, e o próximo da fila se torna o primeiro da fila. Apenas os 4 primeiros elementos da fila são impressos. A fila não é uma thread, mas está relacionada há uma outra thread chamada "colocar_clientes_fila" que tem a função de colocar os novos clientes que chegam no final da fila.

- **Buffer:** O buffer serve para armazenar as comidas que estão prontas e são colocadas pelo chefe e para que os clientes possam se servir, uma vez que um cliente se serve, essa comida é retirada do buffer. O buffer também segue algumas regras: somente o chefe e os clientes acessam o buffer e apenas 1 componente por vez pode acessar o buffer, caso um componente tente acessar o buffer enquanto ele estiver bloqueado (algum componente está acessando ele) esse componente precisará esperar. O tamanho máximo do buffer é 4, uma vez que o máximo de clientes que o restaurante pode ter é 4 e o chefe só cozinha 1 comida para cada, não podendo ultrapassar o número 4.

## Como funcionam o sistema de produto-consumidor usando mutex e semáforos do projeto?

Um sistema de produtor-consumidor é um padrão de projeto bem comum em programação concorrente/paralela para resolver problemas de coordenar ações de múltiplas threads que compartilham um buffer de dados e/ou uma fila. O produtor é responsável por gerar dados e adicioná-los num buffer/fila compartilhada. Ele produz continuamente os dados e os adiciona no buffer/fila compartilhada. Um consumidor é responsável por consumir ou processar os dados do buffer/fila compartilhada. Ele continuamente pega os dados do buffer e performa alguma operação neles.

Os mutex servem para impedir que duas ou mais threads acessem a mesma estrutura de dados ao mesmo tempo.
No caso do Restaurante, existem 3 mutex relacionados a dados.

- **mutexBuffer**: impede que a mesa e o chef, ou duas mesas ou dois chefes acessem o buffer de comidas ao mesmo tempo, impedindo que um cliente pegue uma comida fora do buffer.

- **mutexFila**: impede que duas mesas acessem a fila ao mesmo tempo, impedindo assim que dois clientes iguais existam no restaurante.

- **mutexPrint**: Como existem várias funções de printar o estado do restaurante, esse mutex evita que uma função imprima durante a impressão de outra, evitando, assim, erros gráficos.

Já os semáforos são usados para a sincronização e no caso do Restaurante existem 3 semáforos, sendo eles:

- **semPedidosFeitos**: controla a produção de pedidos pelos clientes. As mesas aguardam até que um pedido seja feito antes de consumir uma comida.

- **semPedidosProntos**: controla a disponibilidade de pedidos prontos para consumo.

- **semFila**: controla a disponibilidade de clientes na fila. Quando um cliente chega, ele sinaliza que há clientes na fila disponíveis para ocupar as mesas

## Como rodar o projeto?

- Primeiramente é necessário baixar a pasta "projetomultithread" em sua máquina;

- Após isso é só rodar os dois seguintes comandos em sequência no terminal:

```c
make

./restaurante.out
```

- Depois disso, os estados do restaurante vão começar a ser printados no terminal;

## O que o projeto printa?

Existe uma função de print que é chamada quando um cliente senta na mesa e quando ele começa a comer. Essa função printa uma fotografia do estado atual do restaurante
Abaixo tem-se um exmeplo

```sh
                    |---------------------------------------------------------|
                    |                                                         |
                    |   |---------------------------------------| |----|      |
                    |   |Camarão  |         |         |         | |Chef|      |
                    |   |---------------------------------------| |----|      |
                    |                                                         |
                    |                                                         |
                    |                                                         |
                    | Cl 30|\`====='/|                Cl 31|\`====='/|        |
                    |      |  Mesa0  |                     |  Mesa1  |        |
                    |                                                         |
--------------------                                                          |
Cl 35 Cl 34 Cl 33 Cl 32                                                       |
--------------------                                                          |
                    |         Mignon                          Frango          |
                    | Cl 29|\`====='/|               Cl  27|\`====='/|        |
                    |      |  Mesa2  |                     |  Mesa3  |        |
                    |                                                         |
                    |---------------------------------------------------------|

```
