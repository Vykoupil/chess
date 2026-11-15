#include "move.h"
#include <stdlib.h>

bool pawn(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    int row_diff = final_row - starting_row;
    int col_diff = abs(final_col - starting_col);
    int final_square = chess_board[final_row][final_col];

    if (piece > 0) 
    {
        if (col_diff == 0 && row_diff == -1 && final_square == EMPTY) return true;
        if (col_diff == 0 && row_diff == -2 && starting_row == 6 && final_square == EMPTY && chess_board[starting_row - 1][starting_col] == EMPTY) return true;
        if (col_diff == 1 && row_diff == -1 && final_square < 0) return true;
        if (col_diff == 1 && row_diff == -1 && final_row == en_passant_row && final_col == en_passant_col) return true;
    }
    else if (piece < 0) 
    {
        if (col_diff == 0 && row_diff == 1 && final_square == EMPTY) return true;
        if (col_diff == 0 && row_diff == 2 && starting_row == 1 && final_square == EMPTY && chess_board[starting_row + 1][starting_col] == EMPTY) return true;
        if (col_diff == 1 && row_diff == 1 && final_square > 0) return true;
        if (col_diff == 1 && row_diff == 1 && final_row == en_passant_row && final_col == en_passant_col) return true;
    }

    return false;
}