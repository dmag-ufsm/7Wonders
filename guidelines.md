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

# Problemas encontrados
* a. O map de recursos está zerando em determinados momentos do jogo (e.g.
  comércio).
    * Isso ainda tá acontecendo acontece com moedas :/
* b. No geral, o comércio está um tanto bugado:
    * Às vezes os recursos não estão sendo atualizados corretamente, inclusive
      as moedas. Tem que ver isso aí.
* c. ~~Alguns recursos estão indo pro valor 255 após algumas jogadas.~~
    * ~~Acho que isso tem a ver com overflow do ```unsigned char``` sendo usado
      no map. Sugestão: trocar pra short ou int; talvez em algum cálculo o
      ```unsigned char``` fique negativo por algum motivo.~~
      * Troquei a quantidade de recursos de ```unsigned char``` para ```int```, que é o tipo mais usual. 
      _Provavelmente_ o under/overflow não vai mais ocorrer. Pode não ser o
      melhor tipo para expressar uma quantidade que não é grande, mas ao menos
      é o que tipicamente causa menos problemas.
* d. Algumas vezes recursos ficam com valor negativo.
    * P: testei várias vezes e ainda não consegui reproduzir isso. Talvez
          esteja relacionado com o problema (a).
* e. ~~Depois da sexta rodada a última carta não é descartada.~~
    * Corrigido. Ainda, a possibilidade de jogar a sétima carta (se o jogador
      tiver essa habilidade) foi adicionada.
* f. ~~Algumas cartas aparecem como jogáveis mesmo sem serem jogáveis.~~
    * ~~Acho que é erro em GetPlayableCards, pois mesmo ao escolhê-las elas não
      são jogadas.~~
    * Corrigido (?). Resta fazer mais testes.

**Aviso: cuidem as cartas que podem ser construídas de graça em caso de
combos.**. 

# TODO
## Main
* Efeitos de Wonders:
    * BuildDiscardFree: baixa uma carta de graça do descarte LOGO APÓS construir o
      estágio 2/3 de Halikarnassus B ou o estágio 2 de Halikarnassus A.
    * CopyGuild: no final do jogo, o player pode copiar uma guilda de um dos
      seus vizinhos. Precisa ter o estágio 3 de Olympia B construído.
