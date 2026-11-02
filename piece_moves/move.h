#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>
#include "../chess_log/logic.h"

extern int en_passant_row;
extern int en_passant_col;
extern bool white_king_moved;
extern bool white_rook_a1_moved;
extern bool white_rook_h1_moved;
extern bool black_king_moved;
extern bool black_rook_a8_moved;
extern bool black_rook_h8_moved;

bool king(int starting_row, int starting_column, int ending_row, int ending_column, int piece);
bool queen(int starting_row, int starting_column, int ending_row, int ending_column, int piece);
bool rook(int starting_row, int starting_column, int ending_row, int ending_column, int piece);
bool bishop(int starting_row, int starting_column, int ending_row, int ending_column, int piece);
bool knight(int starting_row, int starting_column, int ending_row, int ending_column, int piece);
bool pawn(int starting_row, int starting_column, int ending_row, int ending_column, int piece);


#endif