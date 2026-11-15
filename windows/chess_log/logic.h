#ifndef CHESS_LOG_H // Ochrana proti opakování souboru
#define CHESS_LOG_H // Pokud to neni definováno tak se to definuje

#include <stdbool.h>

extern int turn;
extern bool game_over;
extern int result;

// Přiřazování čísel k figurkám jako u židů pro manipulaci
enum Piece {
    EMPTY = 0,
    W_PAWN = 1, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN = -1, B_KNIGHT = -2, B_BISHOP = -3, B_ROOK = -4, B_QUEEN = -5, B_KING = -6
};

// proměnné co budou v logic.c
extern int chess_board[8][8]; 
extern int turn; 
extern bool game_over;
extern int result;

void reset(void); // Funkce pro reset hry
bool check_move(int starting_row, int starting_column, int ending_row, int ending_column); // Číhne jde takhle táhnout a eventuelně provede tah
bool check(int king_skin_color); // Ověřuje zda neni náhodou král v šachu
bool mate(int player); // Ověření zda je vubec jak táhnout
bool under_attack(int target_row, int target_col, int attacker_color); // je pole napadeno
bool geometry(int sr, int sc, int er, int ec, int piece);
bool en_passant(int sr, int sc, int er, int ec, int piece);
void en_passant_take(int er, int ec, int piece, int *v_row, int *v_col, int *cap_piece);
bool castling(int sr, int sc, int er, int ec, int piece);
bool castling_safe(int row, int start_c, int end_c, int color);
void castling_rook(int row, int start_c, int end_c);
bool promotion(int piece, int final_row);
void promote(int final_row, int final_col, int color);

#endif 

//sr = starting row, sc = starting column, er = ending row, ec = ending column¨
