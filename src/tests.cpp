//
// Created by jio on 9/16/19.
//
//
// Como escrever os testes:
//
// TEST(nome_do_teste,tipo_de_teste)
// {
// 		ASSERT_ALGUMA_COISA(valores);
// }
//
// simples assim.
#include <gtest/gtest.h>
#include <player.h>
#include <card.h>
#include <deck.h>

TEST(TEST1, POS_NUM){
    ASSERT_EQ(1,1);
}

TEST(PLAYER, PLAYER_SCORE){
    using namespace DMAG;

    /*
     * Inicialmente, o deck está sendo preenchido com qualquer coisa, pra poder fazer cálculos.
     * Isso será removido logo que implementarmos as funções de "distribuir cartas pros players".
     */
    Player p;
    ASSERT_EQ(p.GetScore(), 20);
    p.ReceiveCard(Card());
    ASSERT_EQ(p.GetScore(), 30);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
