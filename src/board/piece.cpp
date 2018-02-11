#include <ctype.h>
#include <board/piece.h>
#include <stdexcept>

Piece::Piece() : Piece(piece_symbols[TYPE_NONE]) {

}

Piece::Piece(char symbol) {
    if(isupper(symbol))
      color = COLOR_WHITE;
    else
      color = COLOR_BLACK;

    type = TYPE_NONE;
    for(int i = 0; i < 7; i++) {
        if(tolower(symbol) == piece_symbols[i])
            type = static_cast<Type>(i);
    }

    if(tolower(symbol) != piece_symbols[type])
        throw std::runtime_error("Invalid piece symbol");
}

Piece::Piece(Color color, Type type) {
    this->color = color;
    this->type = type;
}

char Piece::get_symbol() {
    if(color == COLOR_WHITE)
        return toupper(piece_symbols[type]);
    else
        return piece_symbols[type];
}

std::string Piece::get_img_path() {
    return "";
}

Color Piece::get_color() {
    return color;
}

Type Piece::get_type() {
    return type;
}

Type& operator++(Type& type) {
    if(type == TYPE_PAWN)
        return type = TYPE_NONE;
    else
        return type = static_cast<Type>(static_cast<int>(type) + 1);
}
