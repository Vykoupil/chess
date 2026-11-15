
#include "raylib.h"
#include "../chess_grap/chess_graphic.h"
#include "../chess_log/logic.h"
#include "../chess_voice_control/voice_controls.h"

#include <stdio.h>
#include <string.h>

Texture2D white_king, white_queen, white_pawn, white_knight, white_bishop, white_rook;
Texture2D black_king, black_queen, black_pawn, black_knight, black_bishop, black_rook;

#define BOARD_SIZE 800
#define SQUARE_SIZE (BOARD_SIZE / 8)
#define OFFSET 100

static int promotion_piece(const char *name) {
    if (strcmp(name, "queen") == 0) return W_QUEEN;
    if (strcmp(name, "rook") == 0) return W_ROOK;
    if (strcmp(name, "bishop") == 0) return W_BISHOP;
    if (strcmp(name, "knight") == 0) return W_KNIGHT;

    return 0;
}

static void finish_turn(void) {
    turn = -turn;

    if (!mate(turn)) {
        game_over = true;
        result = check(turn) ? -turn : 2;
    }
}

static int find_voice_promotion(
    const int before[8][8],
    int color,
    int *out_row,
    int *out_col
){

    int target_row = (color == 1) ? 0 : 7;
    int source_row = (color == 1) ? 1 : 6;

    for (int c = 0; c < 8; c++) {

        if (chess_board[target_row][c] != color * W_QUEEN ||
            before[target_row][c] == color * W_QUEEN) {
            continue;
        }

        for (int sc = c - 1; sc <= c + 1; sc++) {

            if (sc >= 0 && sc < 8 &&
                before[source_row][sc] == color * W_PAWN &&
                chess_board[source_row][sc] == EMPTY) {

                *out_row = target_row;
                *out_col = c;

                return 1;
            }
        }
    }

    return 0;
}

int main(void) {

    InitWindow(BOARD_SIZE + 2 * OFFSET, BOARD_SIZE + 2 * OFFSET, "Chess Board");

    SetTargetFPS(60);

    reset();
    load_figure();

    int selected_row = -1;
    int selected_col = -1;

    bool is_running = false;

    bool promotion_pending = false;
    int promotion_row = -1;
    int promotion_col = -1;

    const char *choices[4] = {
        "queen",
        "rook",
        "bishop",
        "knight"
    };

    Rectangle start_button = {
        910, 475, 85, 50
    };

    bool voice_ready = init_voice("vosk-model-small-en-us-0.15");
    char voice_status[120] = "Voice unavailable (check model and microphone)";

    if (voice_ready) {

        snprintf(voice_status, sizeof voice_status, "Voice ready: say START GAME");

    }

    while (!WindowShouldClose()) {

        bool mouse_clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        Vector2 mouse = GetMousePosition();

        bool button_clicked =mouse_clicked && CheckCollisionPointRec(mouse, start_button);

        if (IsKeyPressed(KEY_R) || button_clicked) {

            is_running = !is_running;

            reset();

            selected_row = -1;
            selected_col = -1;

            promotion_pending = false;
            promotion_row = -1;
            promotion_col = -1;

            snprintf(voice_status, sizeof voice_status, "%s", is_running ? "Game started" : "Game ended");
        }

        if(is_running && !game_over && mouse_clicked && !button_clicked){

            if (promotion_pending) {

                for(int i = 0; i < 4; i++){

                    Rectangle option = {
                        905,
                        255 + i * 47,
                        90,
                        40
                    };

                    if(CheckCollisionPointRec(mouse, option)){

                        chess_board[promotion_row][promotion_col] =
                            turn * promotion_piece(choices[i]);

                        promotion_pending = false;
                        promotion_row = -1;
                        promotion_col = -1;

                        finish_turn();

                        snprintf(voice_status, sizeof voice_status, "Promoted to %s", choices[i]);

                        break;
                    }
                }

            }else if(mouse.x >= OFFSET && mouse.x < OFFSET + BOARD_SIZE && mouse.y >= OFFSET && mouse.y < OFFSET + BOARD_SIZE){

                int col = ((int)mouse.x - OFFSET) / SQUARE_SIZE;

                int row = ((int)mouse.y - OFFSET) / SQUARE_SIZE;

                if (selected_row >= 0) {

                    int moving_piece =
                        chess_board[selected_row][selected_col];

                    if (check_move(selected_row, selected_col, row, col)){

                        bool was_pawn = (moving_piece == turn * W_PAWN && row == (turn == 1 ? 0 : 7));

                        selected_row = -1;
                        selected_col = -1;

                        if (was_pawn) {

                            promotion_pending = true;
                            promotion_row = row;
                            promotion_col = col;

                            snprintf(voice_status, sizeof voice_status, "Choose: queen, rook, bishop, knight");

                        }else{
                            finish_turn();
                        }

                    }else if(chess_board[row][col] * turn > 0){

                        selected_row = row;
                        selected_col = col;

                    }else{

                        selected_row = -1;
                        selected_col = -1;
                    }

                }else if(chess_board[row][col] * turn > 0){

                    selected_row = row;
                    selected_col = col;

                }
            }
        }

        if (voice_ready) {

            char command[160];

            if (process_audio(command, sizeof command)) {

                if (strcmp(command, "start game") == 0) {

                    is_running = true;

                    reset();

                    selected_row = -1;
                    selected_col = -1;

                    promotion_pending = false;
                    promotion_row = -1;
                    promotion_col = -1;

                    snprintf(voice_status, sizeof voice_status, "Game started by voice");

                } else if (strcmp(command, "end game") == 0) {

                    is_running = false;

                    reset();

                    selected_row = -1;
                    selected_col = -1;

                    promotion_pending = false;
                    promotion_row = -1;
                    promotion_col = -1;

                    snprintf(voice_status, sizeof voice_status, "Game ended by voice");

                }else if(strcmp(command, "resign") == 0 && is_running && !game_over){

                    game_over = true;
                    result = -turn;

                    promotion_pending = false;
                    promotion_row = -1;
                    promotion_col = -1;

                    snprintf(voice_status, sizeof voice_status, "Player resigned");

                }else if(is_running && !game_over && promotion_pending){

                    int chosen = promotion_piece(command);

                    if(chosen){

                        chess_board[promotion_row][promotion_col] =
                            turn * chosen;

                        promotion_pending = false;
                        promotion_row = -1;
                        promotion_col = -1;

                        finish_turn();

                        snprintf(voice_status, sizeof voice_status, "Promoted to %s", command);

                    }else{

                        snprintf(voice_status, sizeof voice_status, "Choose: queen, rook, bishop, knight");

                    }

                }else if(is_running && !game_over){

                    int before[8][8];

                    memcpy(before, chess_board, sizeof before);

                    int matches = voice_move(command);

                    if(matches == 1){

                        selected_row = -1;
                        selected_col = -1;

                        if(find_voice_promotion(before, turn, &promotion_row, &promotion_col)){

                            promotion_pending = true;

                            snprintf(voice_status, sizeof voice_status, "Choose: queen, rook, bishop, knight");

                        }else{

                            finish_turn();

                            snprintf(voice_status, sizeof voice_status, "Played: %.90s", command);

                        }

                    }else if(matches > 1){

                        snprintf(voice_status, sizeof voice_status, "Ambiguous: say origin square");

                    }else{

                        snprintf(voice_status, sizeof voice_status, "Invalid: %.90s", command);

                    }
                }
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        draw_board(OFFSET, BOARD_SIZE, selected_row, selected_col, is_running);

        draw_piece(chess_board, OFFSET, SQUARE_SIZE);
        
        DrawRectangleRec(start_button, is_running ? MAROON : DARKGREEN);

        DrawRectangleLinesEx(start_button, 2, BLACK);

        const char *button_text =
            is_running ? "End(R)" : "Start(R)";

        int font_size = 19;

        DrawText(button_text, (int)(start_button.x +(start_button.width - MeasureText(button_text, font_size)) / 2), (int)(start_button.y +(start_button.height - font_size) / 2), font_size, WHITE);

        if(promotion_pending){

            DrawText("PROMOTE", 904, 225, 17, BLACK);

            for(int i = 0; i < 4; i++){

                Rectangle option = {
                    905,
                    255 + i * 47,
                    90,
                    40
                };

                DrawRectangleRec(option, DARKBLUE);

                DrawRectangleLinesEx(option, 1, BLACK);

                DrawText(choices[i], 911, (int)option.y + 11, 17, WHITE);
            }
        }

        DrawText(voice_status, 100, 950, 18, DARKGRAY);

        EndDrawing();
    }

    if(voice_ready){
        free_voice();
    }

    unload_figure();

    CloseWindow();

    return 0;
}
