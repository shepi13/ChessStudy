#include <board/board.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <iostream>
#include <algorithm>

Board::Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 0", true) {

}

Board::Board(std::string fen, bool shredder) {
    set_fen_position(fen, shredder);
}

void Board::set_fen_position(std::string fen, bool shredder) {
    std::istringstream iss(fen);
    std::vector<std::string> tokens((std::istream_iterator<std::string>(iss)),
                                    std::istream_iterator<std::string>());

    if(tokens.size() < 1)
        throw std::runtime_error("Invalid fen");

    //Set Position
    int current_rank = 8, current_file = 1;
    for(auto piece_char : tokens.at(0)) { 
        if(piece_char == '/') {
            if(current_file != 9)
                throw std::runtime_error("Invalid fen");

            current_rank -= 1; current_file = 1;
            continue;
        }
        
        if(current_rank < 1 || current_file > 8)
           throw std::runtime_error("Invalid fen"); 

        if(piece_char > '0' && piece_char < '9')
        {
            current_file += piece_char - '0';
        } else {
            position[current_rank-1][current_file-1] = Piece(piece_char);

            if(position[current_rank-1][current_file-1].get_type() == TYPE_NONE)
                throw std::runtime_error("Invalid fen");

            current_file++;
        }
    }

    //Set Side to Move
    if(tokens.size() >= 2) {
        if(tokens.at(1) == "w")
           side_to_move = COLOR_WHITE;
        else if(tokens.at(1) == "b")
           side_to_move = COLOR_BLACK;
        else
           throw std::runtime_error("Invalid fen"); 
    } else {
        side_to_move = COLOR_WHITE;
    }

    //Set Castling
    if(tokens.size() >= 3) {
        castling = 0;
        if(!shredder) {
            if(tokens.at(2).find('K'))
                castling |= get_bitboard(get_square("h1"));
            if(tokens.at(2).find('Q'))
                castling |= get_bitboard(get_square("a1"));
            if(tokens.at(2).find('k'))
                castling |= get_bitboard(get_square("h8"));
            if(tokens.at(2).find('q'))
                castling |= get_bitboard(get_square("a8"));
        }
        else {
            for(auto castle_char : tokens.at(2)) {
                if(tolower(castle_char) < 'a' || tolower(castle_char) > 'h')
                    throw std::runtime_error("Invalid fen");
                
                Column castle_column = static_cast<Column>(tolower(castle_char) - 'a' + 1);  

                if(isupper(castle_char))
                    castling |= get_bitboard(get_square(castle_column, 1));
                else
                    castling |= get_bitboard(get_square(castle_column, 8));

            }
        }
    } else {
        castling = 0;
        
        //If not specified, assume castling is legal if rooks are on starting squares
        if(get_piece(get_square("a1")).get_type() == TYPE_ROOK)
            castling |= get_bitboard(get_square("a1"));
        if(get_piece(get_square("h1")).get_type() == TYPE_ROOK)
            castling |= get_bitboard(get_square("h1"));
        if(get_piece(get_square("a8")).get_type() == TYPE_ROOK)
            castling |= get_bitboard(get_square("a8"));
        if(get_piece(get_square("h8")).get_type() == TYPE_ROOK)
            castling |= get_bitboard(get_square("h8"));
    }

    //Set Enpassant int
    if(tokens.size() >= 4)
    {
        if(tokens.at(3) == "-")
            ep_square = -1;
        else {
            try {
                ep_square = get_square(tokens.at(3));
            } catch (const std::runtime_error& re) {
                throw std::runtime_error("Invalid fen");
            }
        }
    } else {
        ep_square = -1;
    }

    //Set Halfmove Timer - Set to 0 if missing
    if(tokens.size() >= 5)
    {
        try {
            halfmove_timer = stoi(tokens.at(4));
        } catch(const std::invalid_argument& ia) {
            throw std::runtime_error("Invalid fen");
        }
    } else {
        halfmove_timer = 0;
    }

    //Set Move Number - Set to 0 if missing 
    if(tokens.size() >= 6)
    {
        try {
            move_number = stoi(tokens.at(4));
        } catch(const std::invalid_argument& ia) {
            throw std::runtime_error("Invalid fen");
        }
    } else {
        move_number = 0;
    }
}

std::string Board::fen(bool shredder) { 
    std::string fen_string = "";

    //Position string
    for(int rank = 8; rank >= 1; rank--) {
        for(Column column = A; column <= H; ++column) {
            fen_string += position[rank-1][column-1].get_symbol();
        }
        
        if(rank != 1)
            fen_string += "/";
    }

    for(int i = 0; i < fen_string.length(); i++) {
        if(fen_string[i] == '-') {
            int count = 0;

            for(int j = i; j < fen_string.length(); j++) {
                if(fen_string[j] == '-') {
                    i = j;
                    count++;
                }
                else
                    break;
            }

            fen_string[i] = count + '0';
        }
    }
            
    fen_string.erase(std::remove(fen_string.begin(), fen_string.end(), '-'), fen_string.end());
    fen_string += " ";

    //Color string
    if(side_to_move == COLOR_WHITE)
        fen_string += "w ";
    else
        fen_string += "b ";

    //Castling string
    if(shredder) {
        for(int square = get_square("a1"); square <= get_square("h1"); ++square) {
            if(castling & get_bitboard(square))
                fen_string += toupper(column_names[get_column(square)-1]);
        }

        for(int square = get_square("a8"); square <= get_square("h8"); ++square) {
            if(castling & get_bitboard(square))
                fen_string += tolower(column_names[get_column(square)-1]);
        }
    } else {
        if(castling & get_bitboard(get_square("h1")))
            fen_string += "K";
        if(castling & get_bitboard(get_square("a1")))
            fen_string += "Q";
        if(castling & get_bitboard(get_square("h8")))
            fen_string += "k";
        if(castling & get_bitboard(get_square("a8")))
            fen_string += "q";
        if(fen_string.back() == ' ')
            fen_string += "-";
    }

    fen_string += " ";

    //En passant string
    if(ep_square == -1)
        fen_string += "- ";
    else
        fen_string += square_name(ep_square) + " ";

    //Halfmove timer
    fen_string += std::to_string(halfmove_timer) + " ";

    //Fullmove count
    fen_string += std::to_string(move_number);

    return fen_string;
}

void Board::push_move(Move move) {
    board_stack.push_back(*this);
    move_stack.push_back(move);

    move_number++;
    halfmove_timer++;
    side_to_move = static_cast<Color>(!side_to_move);
    ep_square = -1;

    if(move.get_start() == -1 || move.get_end() == -1)
        return;

    if(get_bitboard(move.get_start()) & castling)
        castling &= ~get_bitboard(move.get_start());

    if(get_piece(move.get_start()).get_type() == TYPE_PAWN) {
        halfmove_timer = 0;

        if(abs(get_rank(move.get_start()) - get_rank(move.get_end())) == 2)
            ep_square = static_cast<int>((move.get_start()+move.get_end()) / 2);
    }

    if(move.get_promotion() == TYPE_NONE)
        set_piece(move.get_end(), get_piece(move.get_start()));
    else
        set_piece(move.get_end(), move.get_promotion());

    set_piece(move.get_start(), Piece());
}

Move Board::pop_move() {
    Move move = move_stack.back();
    *this = board_stack.back();
    return move;
}

Move Board::peek_move() {
    return move_stack.back();
}

std::vector<Move> Board::get_legal_moves(bool pseudo) {
    std::vector<Move> legal_moves;

    for(int rank = 1; rank <= 8; rank++) {
        for(Column column = A; column <= H; ++column) {
            int square = get_square(column, rank);
            Piece piece = get_piece(square);
    
            if(piece.get_color() != side_to_move)
                continue;
            
            switch(piece.get_type()) {
            case TYPE_PAWN: 
                add_pawn_moves(legal_moves, square, piece.get_color());
                break;
            case TYPE_ROOK: 
                add_rook_moves(legal_moves, square);
                break;
            case TYPE_KNIGHT: 
                add_knight_moves(legal_moves, square);
                break;
            case TYPE_BISHOP: 
                add_bishop_moves(legal_moves, square);
                break;
            case TYPE_QUEEN: 
                add_queen_moves(legal_moves, square);
                break;
            case TYPE_KING: 
                add_king_moves(legal_moves, square);
                break;
            default:
                break;
            }
        }
    }


    add_promotions(legal_moves);
    if(!pseudo)
    {
        IsIllegal illegal_functor(*this);
        legal_moves.erase(std::remove_if(legal_moves.begin(), legal_moves.end(),
                                        illegal_functor), legal_moves.end());
    }

    return legal_moves;
}

void Board::add_pawn_moves(std::vector<Move> &moves, int square, Color color) {
    if(color == COLOR_WHITE) {
        if(square+8 > 63)
            throw std::runtime_error("Pawn on last rank");

        //Forward moves
        if(get_piece(square+8).get_type() == TYPE_NONE) {
                moves.push_back(Move(square, square+8));

            if(get_rank(square) == 2 && get_piece(square+16).get_type() == TYPE_NONE)
                moves.push_back(Move(square, square+16));
        }
        //Captures
        if(get_piece(square+7).get_type() != TYPE_NONE && 
            get_piece(square+7).get_color() != color && get_column(square+7) != H)
            moves.push_back(Move(square,square+7));

        
        if(square+9 <= 63 && get_piece(square+9).get_type() != TYPE_NONE && 
            get_piece(square+9).get_color() != color && get_column(square+9) != A)
            moves.push_back(Move(square,square+9));
    } else {
        if(square-8 < 0)
            throw std::runtime_error("Pawn on last rank");

        //Forward moves
        if(get_piece(square-8).get_type() == TYPE_NONE) {
            moves.push_back(Move(square, square-8));

            if(get_rank(square) == 7 && get_piece(square-16).get_type() == TYPE_NONE)
                moves.push_back(Move(square, square-16));
        }
        //Captures
        if(get_piece(square-7).get_type() != TYPE_NONE && 
            get_piece(square-7).get_color() != color && get_column(square-7) != A)
            moves.push_back(Move(square,square-7));

        
        if(square-9 >= 0 && get_piece(square-9).get_type() != TYPE_NONE && 
            get_piece(square-9).get_color() != color && get_column(square-9) != H)
            moves.push_back(Move(square,square-9));
    }
}

void Board::add_rook_moves(std::vector<Move> &moves, int square) {

}

void Board::add_knight_moves(std::vector<Move> &moves, int square) {
    if(square+6 <= 63 && get_column(square+6) < G && 
      (get_piece(square+6).get_type() == TYPE_NONE || 
       get_piece(square+6).get_color() != side_to_move))
        moves.push_back(Move(square, square+6)); 

    if(square+10 <= 63 && get_column(square+10) > B && 
      (get_piece(square+10).get_type() == TYPE_NONE || 
       get_piece(square+10).get_color() != side_to_move))
        moves.push_back(Move(square, square+10)); 

    if(square-6 >= 0 && get_column(square-6) > B && 
      (get_piece(square-6).get_type() == TYPE_NONE || 
       get_piece(square-6).get_color() != side_to_move))
        moves.push_back(Move(square, square-6)); 

    if(square-10 >= 0 && get_column(square-10) < G && 
      (get_piece(square-10).get_type() == TYPE_NONE || 
       get_piece(square-10).get_color() != side_to_move))
        moves.push_back(Move(square, square-10)); 

    if(square+15 <= 63 && get_column(square+15) < H &&
      (get_piece(square+15).get_type() == TYPE_NONE ||
       get_piece(square+15).get_color() != side_to_move))
        moves.push_back(Move(square, square+15)); 

    if(square+17 <= 63 && get_column(square+17) > A &&
      (get_piece(square+17).get_type() == TYPE_NONE ||
       get_piece(square+17).get_color() != side_to_move))
        moves.push_back(Move(square, square+17)); 

    if(square-15 >= 0 && get_column(square-15) > A &&
      (get_piece(square-15).get_type() == TYPE_NONE ||
       get_piece(square-15).get_color() != side_to_move))
        moves.push_back(Move(square, square-15)); 

    if(square-17 >= 0 && get_column(square-17) < H &&
      (get_piece(square-17).get_type() == TYPE_NONE ||
       get_piece(square-17).get_color() != side_to_move))
        moves.push_back(Move(square, square-17)); 
}

void Board::add_bishop_moves(std::vector<Move> &moves, int square) {

}

void Board::add_queen_moves(std::vector<Move> &moves, int square) {

}

void Board::add_king_moves(std::vector<Move> &moves, int square) {

}

void Board::add_promotions(std::vector<Move> &moves) {
    int initial_size = moves.size();

    for(int i = 0; i < initial_size; i++) {
        if(get_piece(moves[i].get_start()).get_type() == TYPE_PAWN && 
            (get_rank(moves[i].get_end()) == 8 || get_rank(moves[i].get_end()) == 1)) {

            int start = moves[i].get_start();
            int end = moves[i].get_end();

            moves[i] = Move(start, end, TYPE_QUEEN);
            moves.push_back(Move(start, end, TYPE_KNIGHT));
            moves.push_back(Move(start, end, TYPE_BISHOP));
            moves.push_back(Move(start, end, TYPE_ROOK));
        }
    }
}

bool Board::is_illegal(Move move) const {
    return false;
}

int Board::get_halfmove_timer() {
    return halfmove_timer;
}

int Board::get_move_number() {
    return move_number;
}

bool Board::can_castle(uint64_t square_mask) {
    return static_cast<bool>(castling & square_mask);
}

Color Board::get_side_to_move() {
    return side_to_move;
}

int Board::get_ep_square() {
    return ep_square;
}

Piece Board::get_piece(int square) {
    return position[get_rank(square)-1][get_column(square)-1];
}

void Board::set_piece(int square, Piece piece) {
    position[get_rank(square)-1][get_column(square)-1] = piece;
}

void Board::clear_board() {
    set_fen_position("8/8/8/8/8/8/8/8 w - - 0 0");
}
