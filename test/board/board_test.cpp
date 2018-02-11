#include "gtest/gtest.h"
#include <board/board.h>

TEST(BoardTestSuite, FenConstructorTest) {
    Board board;
    EXPECT_EQ(COLOR_WHITE, board.get_side_to_move());
    EXPECT_EQ(-1, board.get_ep_square());
    EXPECT_EQ(true, board.can_castle(0x8001000000008001));
    EXPECT_EQ(0, board.get_halfmove_timer());
    EXPECT_EQ(0, board.get_move_number());
    EXPECT_EQ(TYPE_PAWN, board.get_piece(get_square("a2")).get_type());
    EXPECT_EQ(TYPE_NONE, board.get_piece(get_square("a3")).get_type());
}

TEST(BoardTestSuite, GetFenTest) {
    Board board;
    EXPECT_STREQ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0", board.fen().c_str());
    EXPECT_STREQ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 0", board.fen(true).c_str());
}

TEST(BoardTestSuite, PushMoveTest) {
    Board board;
    Move move("a2a4");
    board.push_move(move);
    EXPECT_STREQ("rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1", board.fen().c_str());
}

TEST(BoardTestSuite, PushMoveCastleTest) {
    Board board;
    Move move("a1a4");
    board.push_move(move);
    EXPECT_STREQ("rnbqkbnr/pppppppp/8/8/R7/8/PPPPPPPP/1NBQKBNR b Kkq - 1 1", board.fen().c_str());
}

TEST(BoardTestSuite, PopMoveTest) {
    Board board;
    Move move("a2a4");
    Move move2("a7a5");
    board.push_move(move);
    board.push_move(move2);
    EXPECT_EQ(get_square("a7"), board.pop_move().get_start());
    EXPECT_EQ(get_square("a2"), board.pop_move().get_start());
    EXPECT_STREQ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0", board.fen().c_str());
}
