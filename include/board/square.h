#ifndef SQUARE_H 
#define SQUARE_H

#include <string>

enum Column {A=1, B, C, D, E, F, G, H};
extern const char column_names[8];

Column get_column(int square);
int get_rank(int square);

int get_square(Column column, int rank);
int get_square(std::string name);

std::string square_name(int square);

uint64_t get_bitboard(int square);

Column& operator++(Column& column);
#endif
