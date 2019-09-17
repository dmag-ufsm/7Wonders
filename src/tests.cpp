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

TEST(TEST1, POS_NUM){
    ASSERT_EQ(1,1);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
