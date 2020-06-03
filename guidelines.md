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

# Problemas encontrados:
Encontrou algum problema? Abra um
[issue](https://github.com/dmag-ufsm/Game/issues)! Verifique os issues abertos
e fechados; não hesite comentar novamente num issue fechado caso encontrar um
bug supostamente já corrigido.

* Avisos: 
    * Cuidem as cartas que podem ser construídas de graça em caso de combos.
      Isso pode ser confundido como bug, mas não é.
    * Cuidem as cartas repetidas. Quando uma carta é jogada novamente, ela é
      descartada e o número de moedas do jogador aumenta. Isso pode ser
      confundido com um bug, mas não é.

# TODO
* Verificar se o estágio da maravilha pode ser construído ou não. Lógica
  parecida com o que foi feito para checar se uma carta é jogável ou não.
    * Done! Para verificar se um jogador pode construir ou não o estágio
    da maravilha no turno atual, basta verificar o estado da variável
    ```player[i]->can_build_wonder```.

* Leitura de jogo prévio de arquivo
* Escrever sobre as dependências
