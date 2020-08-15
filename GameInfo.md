O cliente irá mandar a entrada filtrada do jogador ao servidor que irá validar a entrada e passara para a lógica do jogo.
Detalhe de implementação: haverá um thread que continuamente receberá e validara os dados recebidos e incluirá numa fila, essa fila será acessada por multiplos threads e por isso utilizará mutex

## Logica

Ao receber um comando (a direção na qual o jogador está tentando andar), se possivel será executado e caso contrario o estado do jogo não será alterado. Apos a posição do jogador será enviada para todos os jogadores.

# Comunicação

A troca de dados entre o cliente e o servidor será por objetos em formato JSON.
Cada jogador terá um identificador unico, uma string, que estará em todos os pacotes que se refiram a algum jogador porem o servidor ignora essa valor sobreescrevendo com o id da coneção pela qual foi recebida.
No caso da valor name caso o cliente não receba esse valor, deve utilizar o id como nome.

Quando um jogador se conectar pela primeira vez, o servidor deve mandar o tamanho do tabuleiro, os pacotes de respostas com a posição atual de todos os jogadores, e transmitir esse novo jogador a todos os clientes

## Todas as mensagems deve possuir o segunte formato

-   type: {Movment | Response | Remove | Disconnect | Initialize | Fruit} the type of the message
-   message: any of the following messages

### Movment

O pacote que informa ao servidor que o jogador tenta se movimentar deve possuir:

-   direction: `{'L' | 'R' | 'U' | 'D'}` indica a direção do movimento
-   id: identificador unico dos jogadores

o score deve ser incluido toda vez que houver uma alteração, porem pode estar presente mesmo que não ocorra
-   score: inteiro

### Response/Player set

O pacote que o servidor manda para os clientes deve possuir:

-   id
-   position: \[X]\[Y] posição do jogador no tabuleiro com origem no canto superior esquerdo

Podendo possuir, caso o cliente não receba o nome nenhuma vez, utilizará o id como nome

-   name: 

### Remove

Esse pacote contem apenas o id e é transmitido pelo servidor para imformar que um jogador se desconectou

### Disconnect

Possui apenas o id e é um pedido de desconexão mandado ao servidor

### Initialize

Possui o tamanho do tabuleiro que o cliente recebe ao se conectar, todas as frutas e outros jogadores

-   size: {X: Number; Y: Number}

### Fruit
Possui a posição de uma fruta, e um boleano que indica se sera adicionado ou removido

-   add: bool
-   poition:
