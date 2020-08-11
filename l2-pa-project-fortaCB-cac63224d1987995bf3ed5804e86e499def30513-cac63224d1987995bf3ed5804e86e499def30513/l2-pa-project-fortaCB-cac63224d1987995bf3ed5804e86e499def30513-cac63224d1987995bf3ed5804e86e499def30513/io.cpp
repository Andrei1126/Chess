// Numai functii pt afisare
#include <cstdio>
#include <vector>
#include "def.h"
using namespace std;

char *prSq(const int sq){
    static char sqStr[3];
    int file = files[sq], rank = ranks[sq];
    sprintf(sqStr, "%c%c", ('a'+file), ('1'+rank));
    return sqStr;
}

char *prMove(Move move){
    static char moveStr[6];
    int fileFrom = files[move.from];
    int rankFrom = ranks[move.from];
    int fileTo = files[move.to];
    int rankTo = ranks[move.to];
    int promoted = move.promotion;

    if(promoted){
        char pchar = 'q';
        if (isKn[promoted]) pchar = 'n';
        else if(promoted == bR || promoted == wR) pchar = 'r';
        else if(promoted == bB || promoted == wB) pchar = 'b';
        sprintf(moveStr, "%c%c%c%c%c", ('a'+fileFrom), ('1'+rankFrom),  ('a'+fileTo), ('1'+rankTo), pchar);
    }
    else sprintf(moveStr, "%c%c%c%c", ('a'+fileFrom), ('1'+rankFrom),  ('a'+fileTo), ('1'+rankTo));

    return moveStr;
}

void printMove(Move move) {
    cout << "From:" << move.from;
    cout << " To:" << move.to << endl;
    cout << "Captured:" << move.captured;
    cout << " Promotion:" << move.promotion << endl;
    cout << "Castle:" << move.castle;
    cout << " PawnStart:" << move.pawnStart;
    cout << " EnPas:" << move.enPas << endl;
    cout << " Score:" << move.score << endl;
}

void printMoveList(std::vector<Move> list) {
    int score, move;
    printf("Move List: \n");
    for (int i = 0; i < list.size(); ++i){
        score = list[i].score;
        printf("Move %d: %s (score: %d)\n", i+1, prMove(list[i]), score);
    }
    printf("MoveList total moves: %ld\n", list.size());
}

void showSqAttackedBySide(const int side, Board *pos) {
    int rank = 0;
    int file = 0;
    int sq = 0;
    char sideChar[] = "wb-";
    printf("\nSquares attacked by: %c\n", sideChar[side]);
    for(rank = RANK_8; rank >= RANK_1; --rank) {
        for(file = A; file <= H; ++file) {
            sq = FR2SQ(file,rank);
            if(sqAttacked(sq,side,pos) == TRUE)
                printf("X ");
            else
                printf("_ ");
        }
        printf("\n");
    }
    printf("\n");
}

void print120(){
    printf("\n");
    for (int i = 119; i >= 0; --i){
        printf("%5d", Sq120ToSq64[i]);
        if(i%10 == 0) printf("\n");
    }
    printf("\n");
}

void print64(){
    printf("\n");
    for (int i = 63; i >= 0; --i){
        printf("%5d", Sq64ToSq120[i]);
        if(i%8 == 0) printf("\n");
    }
    printf("\n");
}
