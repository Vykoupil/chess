#include "raylib.h"

Texture2D white_king, white_queen, white_pawn, white_knight, white_bishop, white_rook;
Texture2D black_king, black_queen, black_pawn, black_knight, black_bishop, black_rook;

void load_figure()
{
    white_king = LoadTexture("../Image/white/white_king.png");
    white_queen = LoadTexture("../Image/white/white_queen.png");
    white_pawn = LoadTexture("../Image/white/white_pawn.png");
    white_knight = LoadTexture("../Image/white/white_knight.png");
    white_bishop = LoadTexture("../Image/white/white_bishop.png");
    white_rook = LoadTexture("../Image/white/white_rook.png");

    black_king = LoadTexture("../Image/black/black_king.png");
    black_queen = LoadTexture("../Image/black/black_queen.png");
    black_pawn = LoadTexture("../Image/black/black_pawn.png");
    black_knight = LoadTexture("../Image/black/black_knight.png");
    black_bishop = LoadTexture("../Image/black/black_bishop.png");
    black_rook = LoadTexture("../Image/black/black_rook.png");
}

void unload_figure()
{
    UnloadTexture(white_king);
    UnloadTexture(white_queen);
    UnloadTexture(white_pawn);
    UnloadTexture(white_knight);
    UnloadTexture(white_bishop);
    UnloadTexture(white_rook);

    UnloadTexture(black_king);
    UnloadTexture(black_queen);
    UnloadTexture(black_pawn);
    UnloadTexture(black_knight);
    UnloadTexture(black_bishop);
    UnloadTexture(black_rook);
}

void white_figure(int offset, int squareSize, Texture2D king, Texture2D queen, Texture2D pawn, Texture2D knight, Texture2D bishop, Texture2D rook)
{

            //Vykreslování bílích figurek

        //pěšci
        for(int i = 0; i < 8; i++)
        {
            DrawTexture(pawn, offset + i * squareSize, offset + 6 * squareSize, WHITE);
        }

        //věže
        for(int i = 0; i < 8; i+=7)
        {
            DrawTexture(rook, offset + i * squareSize, offset + 7 * squareSize, WHITE);
        }

        //kůň
        for(int i = 1; i < 8; i+=5)
        {
            DrawTexture(knight, offset + i * squareSize, offset + 7 * squareSize, WHITE);
        }

        //střelec
        for(int i = 2; i < 8; i+=3)
        {
            DrawTexture(bishop, offset + i * squareSize, offset + 7 * squareSize, WHITE);
        }

        //dáma
        DrawTexture(queen, offset + 3 * squareSize, offset + 7 * squareSize, WHITE);

        //král
        DrawTexture(king, offset + 4 * squareSize, offset + 7 * squareSize, WHITE);

}

void black_figure(int offset, int squaresize, Texture2D king, Texture2D queen, Texture2D pawn, Texture2D knight, Texture2D bishop, Texture2D rook)
{

    //Vykreslování černých figurek

    //pěšci
    for(int i = 0; i < 8; i++)
    {
        DrawTexture(pawn, offset + i * squaresize, offset + squaresize, WHITE);
    }

    //věže
    for(int i = 0; i < 8; i+=7)
    {
        DrawTexture(rook, offset + i * squaresize, offset, WHITE);
    }

    //koně
    for(int i = 1; i < 8; i+=5)
    {
        DrawTexture(knight, offset + i * squaresize, offset, WHITE);
    }

    //střelec
    for(int i = 2; i < 8; i+=3)
    {
        DrawTexture(bishop, offset + i * squaresize, offset, WHITE);
    }

    //dáma
    DrawTexture(queen, offset + 3 * squaresize, offset, WHITE);

    //král
    DrawTexture(king, offset + 4 * squaresize, offset, WHITE);

}

int main()
{
    const int board = 800;
    const int offset = 50;
    
    InitWindow(board + 2 * offset, board + 2 * offset, "Chess board");
    SetTargetFPS(60);

    load_figure();
    
    // boarda
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GRAY); // Základní pozadí okna

        int squareSize = board / 8; // Každé políčko bude mít přesně 100x100 pixelů

        // Cyklus pro 8 řádků (shora dolů)
        for (int r = 0; r < 8; r++)
        {
            // Cyklus pro 8 sloupců (zleva doprava)
            for (int c = 0; c < 8; c++)
            {
                int x = offset + c * squareSize;
                int y = offset + r * squareSize;
                // Střídání barev políček
                if ((r + c) % 2 == 0)
                {
                    // Světlá políčka (Raylib má barvu BEIGE)
                    DrawRectangle(x, y, squareSize, squareSize, BEIGE);
                }
                else
                {
                    // Tmavá políčka (Raylib má barvu BROWN)
                    DrawRectangle(x, y, squareSize, squareSize, BROWN);
                }
            }
        }

        DrawText("Chess Board", board / 2 - 75, board / 2 - 400, 40, BLACK);
            
        // Název pole
        char chars[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
        int numbers[] = {8, 7, 6, 5, 4, 3, 2, 1};

        for(int i = 0; i < 8; i++)
        {
            DrawText(TextFormat("%c", chars[i]), offset + i * squareSize + squareSize / 2 - 10, offset + board + 10, 20, BLACK);
            DrawText(TextFormat("%d", numbers[i]), offset - 30, offset + i * squareSize + squareSize / 2 - 10, 20, BLACK);
        }

        white_figure(offset, squareSize, white_king, white_queen, white_pawn, white_knight, white_bishop, white_rook);
        black_figure(offset, squareSize, black_king, black_queen, black_pawn, black_knight, black_bishop, black_rook);

        EndDrawing();
    }

    unload_figure();
    CloseWindow();
    return 0;
}