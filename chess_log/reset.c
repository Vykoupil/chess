#include "logic.h"
#include "../piece_moves/move.h"
#include <stdio.h>
#include <stdbool.h>


int chess_board[8][8];

int turn = 1;
bool game_over = false;
int result = 0;

int en_passant_row = -1;
int en_passant_col = -1;

bool white_king_moved = false;
bool black_king_moved = false;

bool white_rook_a1_moved = false;
bool white_rook_h1_moved = false;
bool black_rook_a8_moved = false;
bool black_rook_h8_moved = false;

void reset()
{
    int initial_board[8][8] = {
        {-4, -2, -3, -5, -6, -3, -2, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 2, 3, 5, 6, 3, 2, 4}
    };

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            chess_board[r][c] = initial_board[r][c];
        }
    }
    
    turn = 1;
    game_over = false;
    result = 0;

    en_passant_row = -1;
    en_passant_col = -1;

    white_king_moved = false;
    black_king_moved = false;

    white_rook_a1_moved = false;
    white_rook_h1_moved = false;
    black_rook_a8_moved = false;
    black_rook_h8_moved = false;
}




