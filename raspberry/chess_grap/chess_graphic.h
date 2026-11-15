#ifndef CHESS_GRAPHIC_H
#define CHESS_GRAPHIC_H

#include "raylib.h"
#include <stdbool.h>

extern Texture2D white_king, white_queen, white_pawn, white_knight, white_bishop, white_rook;
extern Texture2D black_king, black_queen, black_pawn, black_knight, black_bishop, black_rook;

void load_figure();
void unload_figure();
void draw_board(int offset, int board_size, int selected_row, int selected_col, bool is_running);
void draw_piece(int board[8][8], int offset, int squareSize);

#endif 