#include "gtest/gtest.h"
#include <board/square.h>

TEST(SquareTestSuite, GetColumnTest) {
    EXPECT_EQ(C, get_column(get_square("C5")));
    EXPECT_EQ(A, get_column(0));
    EXPECT_EQ(H, get_column(63));
}

TEST(SquareTestSuite, GetRankTest) {
    EXPECT_EQ(3, get_rank(get_square("E3")));
    EXPECT_EQ(1, get_rank(0));
    EXPECT_EQ(8, get_rank(63));
}

TEST(SquareTestSuite, GetSquareByLocationTest) {
    EXPECT_EQ(get_square("F7"), get_square(F, 7));
    EXPECT_EQ(get_square("A1"), get_square(A, 1));
    EXPECT_EQ(get_square("H8"), get_square(H, 8));
}

TEST(SquareTestSuite, GetSquareByNameTest) {
    EXPECT_EQ(27, get_square("D4"));
    EXPECT_EQ(0, get_square("a1"));
    EXPECT_EQ(63, get_square("h8"));
}

TEST(SquareTestSuite, SquareByNameFailTest) {
    EXPECT_THROW(get_square("J7"), std::runtime_error);
    EXPECT_THROW(get_square("A9"), std::runtime_error);
}

TEST(SquareTestSuite, GetSquareFailTest) {
    EXPECT_THROW(get_square(static_cast<Column>(-1), 5), std::runtime_error);
    EXPECT_THROW(get_square(A, 0), std::runtime_error);
    EXPECT_THROW(get_square(H, 9), std::runtime_error);
}

TEST(SquareTestSuite, SquareNameTest) {
    EXPECT_EQ("g2", square_name(14));
    EXPECT_EQ("a1", square_name(0));
    EXPECT_EQ("h8", square_name(63));
}

TEST(SquareTestSuite, GetBitboardTest) {
    EXPECT_EQ(32, get_bitboard(get_square("F1")));
    EXPECT_EQ(16, get_bitboard(get_square("E1")));
    EXPECT_EQ(0x8000000000000000LL, get_bitboard(63));
}
