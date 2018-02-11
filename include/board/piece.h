#ifndef PIECE_H
#define PIECE_H

#include <string>

enum Color {COLOR_WHITE, COLOR_BLACK};
enum Type {TYPE_NONE, TYPE_ROOK, TYPE_KNIGHT, TYPE_BISHOP, TYPE_KING, TYPE_QUEEN, TYPE_PAWN};
const char piece_symbols[7] = {'-', 'r', 'n', 'b', 'k', 'q', 'p'};

Type& operator++(Type& type);

class Piece {
private:
    Color color;
    Type type;

public:
    Piece();
    Piece(char symbol);
    Piece(Color color, Type type);

    Color get_color();
    Type get_type();

    char get_symbol();
    std::string get_img_path();
};

#endif
