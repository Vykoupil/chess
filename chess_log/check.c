#include "logic.h"
#include <stdbool.h>
#include <stdlib.h>
#include "../piece_moves/move.h"


bool check(int king_color)
{
    int king_piece = (king_color == 1) ? W_KING : B_KING;
    int king_row = -1, king_col = -1;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (chess_board[r][c] == king_piece) {
                king_row = r;
                king_col = c;
                break;
            }
        }
    }

    if (king_row == -1) return false;

    int enemy_color = -king_color;
    return under_attack(king_row, king_col, enemy_color);
}