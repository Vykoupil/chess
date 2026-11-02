#include "move.h"
#include <stdio.h>
#include <stdlib.h>

bool king(int starting_row, int starting_col, int final_row, int final_col, int piece)

{
    int row_diff = abs(final_row - starting_row);
    int col_diff = abs(final_col - starting_col);

    
    if ((row_diff == 0 && col_diff == 2) && (piece > 0 && starting_row == 7 && starting_col == 4 && !white_king_moved))
    {
        if ((final_col == 6 && !white_rook_h1_moved) && (chess_board[7][6] == EMPTY && chess_board[7][5] == EMPTY)) return true;
        if ((final_col == 2 && !white_rook_a1_moved) && (chess_board[7][1] == EMPTY && chess_board[7][2] == EMPTY && chess_board[7][3] == EMPTY)) return true;
        return false;
    }
    else if ((row_diff == 0 && col_diff == 2) && (piece < 0 && starting_row == 0 && starting_col == 4 && !black_king_moved))
    {
        if ((final_col == 6 && !black_rook_h8_moved) && (chess_board[0][5] == EMPTY && chess_board[0][6] == EMPTY)) return true;
        if ((final_col == 2 && !black_rook_a8_moved) && (chess_board[0][3] == EMPTY && chess_board[0][2] == EMPTY && chess_board[0][1] == EMPTY)) return true;
        return false;
    }

    bool is_valid = (row_diff <= 1 && col_diff <= 1) && (row_diff > 0 || col_diff > 0);
    if (!is_valid) return false;

    int final_square = chess_board[final_row][final_col];
    if (piece < 0 && final_square < 0) return false;
    if (piece > 0 && final_square > 0) return false;

    return true;
}