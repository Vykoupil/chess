#include "voice_controls.h"
#include "../chess_log/logic.h"
#include "../piece_moves/move.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static int square(const char *s, int *row, int *col) {
    if (!s || strlen(s)!=2) return 0;
    char file=(char)tolower((unsigned char)s[0]);
    if(file<'a'||file>'h'||s[1]<'1'||s[1]>'8') return 0;
    *col=file-'a'; *row='8'-s[1]; return 1;
}
static int piece_type(const char *s) {
    if(!strcmp(s,"pawn") || !strcmp(s,"born")) return W_PAWN;
    if(!strcmp(s,"knight")||!strcmp(s,"night")) return W_KNIGHT;
    if(!strcmp(s,"bishop")) return W_BISHOP;
    if(!strcmp(s,"rook")) return W_ROOK;
    if(!strcmp(s,"queen")) return W_QUEEN;
    if(!strcmp(s,"king")) return W_KING;
    return 0;
}
/* Testing check_move changes the board and castling state. Save and restore all. */
static int trial(int sr,int sc,int er,int ec) {
    int saved[8][8]; memcpy(saved,chess_board,sizeof saved);
    int ep_r=en_passant_row,ep_c=en_passant_col;
    bool wk=white_king_moved,bk=black_king_moved;
    bool wa=white_rook_a1_moved,wh=white_rook_h1_moved;
    bool ba=black_rook_a8_moved,bh=black_rook_h8_moved;
    int ok=check_move(sr,sc,er,ec);
    memcpy(chess_board,saved,sizeof saved);
    en_passant_row=ep_r;en_passant_col=ep_c;
    white_king_moved=wk;black_king_moved=bk;
    white_rook_a1_moved=wa;white_rook_h1_moved=wh;
    black_rook_a8_moved=ba;black_rook_h8_moved=bh;
    return ok;
}
int voice_move(const char *command) {
    char words[7][32]={{0}};
    int n=0; const char *p=command;
    while (*p && n<7) {
        while(*p && !isalnum((unsigned char)*p)) p++;
        if(!*p) break;
        int j=0;
        while(isalnum((unsigned char)*p)) {
            if(j<31) words[n][j++]=(char)tolower((unsigned char)*p);
            p++;
        }
        n++;
    }
    /* Vosk usually transcribes E4 as "e four". Combine letter + number. */
    for (int i=1;i<n-1;i++) {
        if(strlen(words[i])!=1 || words[i][0]<'a' || words[i][0]>'h') continue;
        const char *nums[]={"one","two","three","four","five","six","seven","eight"};
        for(int k=0;k<8;k++) if(!strcmp(words[i+1],nums[k])) {
            words[i][1]=(char)('1'+k); words[i][2]=0;
            for(int j=i+1;j<n-1;j++) strcpy(words[j],words[j+1]);
            n--; break;
        }
    }
    if(n!=3 && n!=4) return 0;
    int type=piece_type(words[0]); if(!type) return 0;
    int er,ec,sr=-1,sc=-1;
    if(strcmp(words[n-2],"to") || !square(words[n-1],&er,&ec)) return 0;
    if(n==4 && !square(words[1],&sr,&sc)) return 0;
    int matches=0,from_r=-1,from_c=-1;
    for(int r=0;r<8;r++) for(int c=0;c<8;c++) {
        if(sr>=0 && (sr!=r||sc!=c)) continue;
        if(chess_board[r][c]!=type*turn) continue;
        if(trial(r,c,er,ec)) { matches++;from_r=r;from_c=c; }
    }
    if(matches==1) return check_move(from_r,from_c,er,ec) ? 1:0;
    return matches;
}
