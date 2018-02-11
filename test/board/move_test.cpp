#include "gtest/gtest.h"
#include <board/move.h>

TEST(MoveTestSuite, GetUCITest) {
    Move promote(get_square("h7"), get_square("h8"), TYPE_QUEEN);
    EXPECT_EQ("h7h8q", promote.get_uci());
    
    Move knight_move(get_square("g1"), get_square("f3"));
    EXPECT_EQ("g1f3", knight_move.get_uci());
}

TEST(MoveTestSuite, UCINullMove) {
    Move nullmove(-1, -1, TYPE_NONE);
    EXPECT_EQ("0000", nullmove.get_uci());

    Move nullmove2("0000");
    EXPECT_EQ(-1, nullmove2.get_start());
    EXPECT_EQ(-1, nullmove2.get_end());
    EXPECT_EQ(TYPE_NONE, nullmove2.get_promotion());
}

TEST(MoveTestSuite, UCIConstructorTest) {
    Move promote("h7h8q");
    EXPECT_EQ(get_square("h7"), promote.get_start());
    EXPECT_EQ(get_square("h8"), promote.get_end());
    EXPECT_EQ(TYPE_QUEEN, promote.get_promotion());
}

TEST(MoveTestSuite, InvalidUCITest) {
    EXPECT_THROW(Move invalid("a7a8c"), std::runtime_error);
    EXPECT_THROW(Move invalid("b8b9"), std::runtime_error);
    EXPECT_THROW(Move invalid("c0c1"), std::runtime_error);
    EXPECT_THROW(Move invalid("j5j8"), std::runtime_error);
    EXPECT_THROW(Move invalid("0d1e"), std::runtime_error);
}
