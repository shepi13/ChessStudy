#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <board/square.h>
#include <board/piece.h>

class Move {
private:
    int start;
    int end;
    Type promotion_type;
public:
    Move(std::string uci);
    Move(int start, int end);
    Move(int start, int end, Type promotion);

    std::string get_uci();
    int get_start();
    int get_end();
    Type get_promotion();
};

#endif
