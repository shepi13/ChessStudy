#include "gtest/gtest.h"
#include <board/piece.h>

TEST(PieceTestSuite, SymbolConstructorTest) {
    Piece white_bishop('B');
    Piece none('-'); 

    EXPECT_EQ(TYPE_BISHOP, white_bishop.get_type());
    EXPECT_EQ(COLOR_WHITE, white_bishop.get_color());
    
    EXPECT_EQ(TYPE_NONE, none.get_type());
}

TEST(PieceTestSuite, SymbolConstructorFail) {
    EXPECT_THROW(Piece invalid('C'), std::runtime_error);
}

TEST(PieceTestSuite, GetSymbolTest) {
    Piece white_king(COLOR_WHITE, TYPE_KING);
    Piece black_rook(COLOR_BLACK, TYPE_ROOK); 
    Piece none(COLOR_WHITE, TYPE_NONE);

    EXPECT_EQ('K', white_king.get_symbol());
    EXPECT_EQ('r', black_rook.get_symbol());
    EXPECT_EQ('-', none.get_symbol());
}
