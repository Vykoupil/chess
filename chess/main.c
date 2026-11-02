#include "raylib.h"
#include "../chess_grap/chess_graphic.h"
#include "../chess_log/logic.h"
#include "../chess_voice_control/voice_controls.h"
#include <stdio.h>

Texture2D white_king, white_queen, white_pawn, white_knight, white_bishop, white_rook;
Texture2D black_king, black_queen, black_pawn, black_knight, black_bishop, black_rook;

#define BOARD_SIZE 800
#define SQUARE_SIZE (BOARD_SIZE / 8)
#define OFFSET 100

int main(void)
{
    InitWindow(BOARD_SIZE + 2 * OFFSET, BOARD_SIZE + 2 * OFFSET, "Chess Board");
    SetTargetFPS(60);
    reset();
    load_figure();

    int selected_row = -1;
    int selected_col = -1;
    bool is_running = false;
    Rectangle start_button = {910, 475, 85, 50};

    bool voice_ready = init_voice("vosk-model-small-en-us-0.15");
    char voice_status[120] = "Voice unavailable (check DLL and model)";
    if (voice_ready) snprintf(voice_status, sizeof voice_status, "Voice ready: Pawn to E4");

    while (!WindowShouldClose()) {
        bool mouse_clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        Vector2 mouse = GetMousePosition();
        bool button_clicked = mouse_clicked && CheckCollisionPointRec(mouse, start_button);

        if (IsKeyPressed(KEY_R) || button_clicked) {
            is_running = !is_running;
            reset();
            selected_row = selected_col = -1;
        }

        if (is_running && !game_over && mouse_clicked && !button_clicked) {
            if (mouse.x >= OFFSET && mouse.x < OFFSET + BOARD_SIZE &&
                mouse.y >= OFFSET && mouse.y < OFFSET + BOARD_SIZE) {
                int col = ((int)mouse.x - OFFSET) / SQUARE_SIZE;
                int row = ((int)mouse.y - OFFSET) / SQUARE_SIZE;

                if (selected_row >= 0 && check_move(selected_row, selected_col, row, col)) {
                    turn = -turn;
                    selected_row = selected_col = -1;
                    if (!mate(turn)) {
                        game_over = true;
                        result = check(turn) ? -turn : 2;
                    }
                } else if (chess_board[row][col] * turn > 0) {
                    selected_row = row;
                    selected_col = col;
                } else {
                    selected_row = selected_col = -1;
                }
            }
        }

        if (voice_ready) {
            char command[160];
            if (process_audio(command, sizeof command)) {
                if (is_running && !game_over) {
                    int matches = voice_move(command);
                    if (matches == 1) {
                        turn = -turn;
                        selected_row = selected_col = -1;
                        if (!mate(turn)) {
                            game_over = true;
                            result = check(turn) ? -turn : 2;
                        }
                        snprintf(voice_status, sizeof voice_status, "Played: %.90s", command);
                    } else if (matches > 1) {
                        snprintf(voice_status, sizeof voice_status, "Ambiguous: say origin square");
                    } else {
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
        const char *button_text = is_running ? "End(R)" : "Start(R)";
        int font_size = 19;
        DrawText(button_text,
                 (int)(start_button.x + (start_button.width - MeasureText(button_text, font_size)) / 2),
                 (int)(start_button.y + (start_button.height - font_size) / 2),
                 font_size, WHITE);
        DrawText(voice_status, 100, 950, 18, DARKGRAY);
        EndDrawing();
    }

    if (voice_ready) free_voice();
    unload_figure();
    CloseWindow();
    return 0;
}
