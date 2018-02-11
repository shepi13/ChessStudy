#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <string>
#include <vector>
#include <board/move.h>
#include <board/piece.h>
#include <board/square.h>

class Board {
private:
    Piece position[8][8];
    Color side_to_move;
    int ep_square;
    uint64_t castling;
    int move_number;
    int halfmove_timer;

    std::vector<Move> move_stack;
    std::vector<Board> board_stack;

    void add_pawn_moves(std::vector<Move> &moves, int start, Color color);
    void add_rook_moves(std::vector<Move> &moves, int start);
    void add_knight_moves(std::vector<Move> &moves, int start);
    void add_bishop_moves(std::vector<Move> &moves, int start);
    void add_queen_moves(std::vector<Move> &moves, int start);
    void add_king_moves(std::vector<Move> &moves, int start);
    void add_promotions(std::vector<Move> &moves);

public:
    Board();
    Board(std::string fen, bool shredder=false);

    std::vector<Move> get_legal_moves(bool pseudo=false);

    void push_move(Move move);
    Move pop_move();
    Move peek_move();

    void set_fen_position(std::string fen, bool shredder=false);
    std::string fen(bool shredder=false);

    Move parse_san(std::string san);
    std::string get_san(Move move);

    bool is_draw_claimable();
    bool is_insufficient_material();
    bool is_stalemate();
    bool is_checkmate();

    std::string result();

    int get_halfmove_timer();
    int get_move_number();
    bool can_castle(uint64_t square_mask);
    Color get_side_to_move();
    int get_ep_square();

    Piece get_piece(int square);
    void set_piece(int square, Piece piece);

    void clear_board();
    bool is_illegal(Move move) const;

};

//Functor used to remove illegal moves
class IsIllegal {
private:
    const Board current_board;
public:
    IsIllegal(const Board &board) : current_board(board) {}
    bool operator()(Move const& move) {
        return current_board.is_illegal(move);
    }
};
    
#endif
