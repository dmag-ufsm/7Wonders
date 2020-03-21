# Diretrizes

Todos os atributos e métodos serão escritos em inglês.
As funções deverão começar com uma ação, escrita em CamelCase;
ex, função "comprar recurso":
<type> PurchaseResource(args);

Atributos deverão ser escritos em snake_case;
ex, atributo "fichas de conflito":
<type> conflict_markers;

# Referências
* [Regras](https://waa.ai/O48v)
* [Regras rápidas](https://waa.ai/O48z)
* [Informações sobre cartas e maravilhas (tabuladas)](https://github.com/dmag-ufsm/Game/tree/master/references)

# TODO
## Main
* Ver TODO na linha 348!
    * Para tirar o "burden" de fazer isso em player, a verificação de jogadas
      válidas em um turno X poderia ser feita aqui mesmo (também faz mais
      sentido). Em outras palavras, certas ações devem ser restringidas até o
      momento certo.
    * Escrever os "stats" do jogo num arquivo. 
    * Não sei se é válido já que são os bots que vão jogar, mas talvez seja
      interessante fazer com que seja possível "repetir" outra jogada caso uma
      ação tenha retornado false. Isso facilita para players humanos, já que é
      possível fazer uma jogada sem notar que ela dará errado; no atual esquema
      do loop, o turno do jogador é perdido independentemente da jogada dando
      certo ou não.

## Player
* Aparentemente está ok -- foram feitos alguns testes usando um
  player. Resta terminar de implementar a main para que seja possível simular
  um jogo real e assim buscar possíveis erros.

# Classe player:

## Contributors:
### Pedro
### Rafael


Atributos:
- Tabuleiro - uchar
- Moedas - uchar
- Cartas na mão - array
- Cartas jogadas - array
- Recursos - map<recurso do enum, quantidade do recurso>
- Escudos - uchar
- Fichas de Conflito - signed char
- Estágios da maravilha construídos - uchar
- Pontos de vitória - uchar
- Jogadores vizinhos - DMAG::Player

Métodos:
- Construir maravilha - void
- Construir estrutura - void
- Construir guilda - void
- Comprar recurso - int 
- Receber cartas - array
- Descartar - DMAG::Card
- Combate - int
- Calcular pontuação - int

# Classe card:
## Contributors:
### Emilio

Atributos:
- Tipo - uchar
- ID - uchar
- Custo - array
- Recursos necessários - map<recurso do enum, quantidade do recurso>
- Recursos fornecidos - map<recurso do enum, quantidade do recurso>
- Pontos de vitória - uchar
- Poder militar - uchar
- Especial - a definir
- Era - uchar
- Unidades por qtd. de jogadores - nada
- Grátis com - DMAG::Card
- Grátis para - array<DMAG::Card>

Métodos:
- Pode ser comprada?

# Classe game:

## Contributor
### Julia
### Julio
### Rafael


Atributos:
- Jogadores - array<DMAG::Player>
- Número de jogadores - uchar
- Era - uchar
- Rodada - uchar
- Timer da rodada - nada
- Baralho - array<deque<DMAG::Card>>
- Pilha de descarte - array<DMAG::Card>

Métodos:
- Em jogo - bool // vai ser chamado no while() do loop principal
- Próxima rodada - void
- Novo jogo - void
- Distribuir cartas - void
- Trocar cartas - void
- Distribuir maravilhas - void
- Escrever em arquivo
- Ler dos arquivos

# Classe wonder:
## Contributor
### Qualquer


Atributos:
- ID - int
- Nome - string
- Recurso produzido - int?
- Efeitos (especiais) - lista de int?
- Estágio da maravilha - int
- ...

Métodos:
- Pega nome - std::string
- Pega produção - int?
- Pega estágio - int
- Pega efeitos - std::list<int> ?
- Adiciona estágio - void
- Recursos necessários para construir o estágio - ?
- ...
