#include "board/move.h"
#include <ctype.h>
#include <stdexcept>

Move::Move(std::string uci) {
    if(uci == "0000")
    {
        start = -1;
        end = -1;
        promotion_type = TYPE_NONE;
    }
    else {
        try {
            start = get_square(uci.substr(0, 2));
            end = get_square(uci.substr(2, 2));
        } catch (const std::runtime_error& re) {
            throw std::runtime_error("Invalid uci");
        }

        promotion_type = TYPE_NONE;

        if(uci.length() >= 5) {
            for(int i = 0; i < 7; i++) {
                if(piece_symbols[i] == uci[4])
                    promotion_type = static_cast<Type>(i);
            }
            if(promotion_type == TYPE_NONE)
                throw std::runtime_error("Invalid uci");
        }
    }
}

Move::Move(int start, int end) {
    this->start = start;
    this->end = end;
    promotion_type = TYPE_NONE;
}

Move::Move(int start, int end, Type promotion) {
    this->start = start;
    this->end = end;
    this->promotion_type = promotion;
}

std::string Move::get_uci() {
    std::string promotion_char;

    if(start < 0 || end < 0)
        return "0000";

    if(promotion_type == TYPE_NONE)
        promotion_char = "";
    else
        promotion_char = tolower(piece_symbols[promotion_type]);

    return square_name(start) + square_name(end) + promotion_char;
}

int Move::get_start() {
    return start;
}

int Move::get_end() {
    return end;
}

Type Move::get_promotion() {
    return promotion_type;
}
