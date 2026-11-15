#!/bin/bash

export PATH=/ucrt64/bin:/usr/bin:$PATH
cd /c/Chess/chess || exit 1

mkdir -p dll_backup

# Schovat DLL, které překážejí kompilátoru
mv libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll dll_backup/ 2>/dev/null

# Kompilace
gcc main.c ../chess_grap/graphic.c ../chess_log/*.c ../piece_moves/*.c \
    ../chess_voice_control/voice_in.c ../chess_voice_control/voice_move.c \
    -o chess.exe -L. -lraylib -lgdi32 -lwinmm

result=$?

# Vrátit DLL i při chybě kompilace
mv dll_backup/*.dll . 2>/dev/null

if [ "$result" -eq 0 ]; then
    ./chess.exe
else
    echo "Kompilace selhala."
    exit "$result"
fi