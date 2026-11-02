#include "logic.h"
#include "../piece_moves/move.h"
#include <stdlib.h>

bool geometry(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    int abs_p = abs(piece);
    
    switch (abs_p) {
        case W_KNIGHT: return knight(starting_row, starting_col, final_row, final_col, piece);
        case W_ROOK:   return rook(starting_row, starting_col, final_row, final_col, piece);
        case W_BISHOP: return bishop(starting_row, starting_col, final_row, final_col, piece);
        case W_QUEEN:  return queen(starting_row, starting_col, final_row, final_col, piece);
        case W_PAWN:   return pawn(starting_row, starting_col, final_row, final_col, piece);
        case W_KING:   return king(starting_row, starting_col, final_row, final_col, piece);
        default:       return false;
    }
}