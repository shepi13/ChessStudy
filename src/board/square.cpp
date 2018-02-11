#include <board/square.h>
#include <ctype.h>
#include <stdexcept>

const char column_names[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

Column get_column(int square) {
    return static_cast<Column>(square % 8 + 1);        
}

int get_rank(int square) {
    return square / 8 + 1;
}

int get_square(std::string name) {
    Column column = static_cast<Column>(tolower(name[0]) - 'a' + 1);
    int rank = name[1] - '0';
    return get_square(column, rank);
}

int get_square(Column column, int rank) {
    int square_value = column-1 + (rank-1)*8;

    if(rank < 1 || rank > 8 || column < A || column > H)
        throw std::runtime_error("Invalid square");

    return square_value;
}

std::string square_name(int square) {
    return column_names[get_column(square)-1] + std::to_string(get_rank(square));
}

uint64_t get_bitboard(int square) {
    return (1LL << square);
}

Column& operator++(Column& column) {
    return column = static_cast<Column>(static_cast<int>(column) + 1);
}
