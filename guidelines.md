# Diretrizes

Todos os atributos e métodos serão escritos em inglês.
As funções deverão começar com uma ação, escrita em CamelCase
ex, função "comprar recurso":
<type> PurchaseResource(args);

Atributos deverão ser escritos em snake_case
Ex, atributo "fichas de conflito":
<type> conflict_markers;



# Classe player:

Atributos:
- Tabuleiro
- Moedas
- Cartas na mão
- Cartas jogadas
- Recursos
- Escudos
- Fichas de Conflito
- Estágios da maravilha construídos
- Pontos de vitória
- Jogadores vizinhos

Métodos:
- Construir maravilha
- Construir estrutura
- Construir guilda
- Comprar recurso
- Receber cartas
- Descartar
- Combate
- Calcular pontuação

# Classe card:

Atributos:
- Tipo
- Custo
- Recursos
- Pontos de vitória
- Poder militar
- Especial
- Era
- Unidades por qtd. de jogadores
- Grátis com
- Grátis para

Métodos:
- Pode ser comprada?

# Classe game:

Atributos:
- Jogadores
- Número de jogadores
- Era
- Rodada
- Timer da rodada
- Baralho

Métodos:
- Em jogo
- Próxima rodada
- Novo jogo
- Distribuir cartas
- Distribuir maravilhas

# Classe logger:

Atributos:

Métodos:
