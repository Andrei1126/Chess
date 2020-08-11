
#include "def.h"

int sqAttacked(int sq, int side, Board *pos) {
    int pce, i, tSq, dir;
    ASSERT(sqOnBoard(sq));
    ASSERT(sideValid(side));
    ASSERT(checkBoard(pos));
    // pawns
    if(side == WHITE) {
        if(pos->pieces[sq-11] == wP || pos->pieces[sq-9] == wP)
            return TRUE;
    } else
        if(pos->pieces[sq+11] == bP || pos->pieces[sq+9] == bP)
            return TRUE;
    // knights
    for(i = 0; i < 8; i++){
        pce =pos->pieces[sq+KnDir[i]];
        if (pce != OFFBOARD && isKn[pce] && pieceCol[pce] == side)
            return TRUE;
    }
    // rooks, queens
    for(i = 0; i < 4; i++){
        dir = RkDir[i];
        tSq = sq + dir;
        pce = pos->pieces[tSq];
        while(pce != OFFBOARD){
            if(pce != EMPTY){
                if (isRQ[pce] && pieceCol[pce] == side)
                    return TRUE;
                break;
            }
            tSq += dir;
            pce = pos->pieces[tSq];
        }
    }
    // bishop, queens
    for(i = 0; i < 4; i++){
        dir = BiDir[i];
        tSq = sq + dir;
        pce = pos->pieces[tSq];
        while(pce != OFFBOARD){
            if(pce != EMPTY){
                if (isBQ[pce] && pieceCol[pce] == side)
                    return TRUE;
                break;
            }
            tSq += dir;
            pce = pos->pieces[tSq];
        }
    }
    // kings
    for(i = 0; i < 8; i++){
        pce = pos->pieces[sq+KiDir[i]];
        if (pce != OFFBOARD && isKi[pce] && pieceCol[pce] == side)
            return TRUE;
    }
    return FALSE;
}
// am pus toate astea aici ca era fisierul mai gol
const int KnDir[8] = { -8, -19,	-21, -12, 8, 19, 21, 12 };
const int RkDir[4] = { -1, -10,	1, 10 };
const int BiDir[4] = { -9, -11, 11, 9 };
const int KiDir[8] = { -1, -10,	1, 10, -9, -11, 11, 9 };
int pieceVal[13] = { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000  };
bool isPieceBig[13] = { FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE };
bool isPieceMaj[13] = { FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE };
bool isPieceMin[13] = { FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE };
int pieceCol[13] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };
const int castlePerm[120] = {   15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
                                15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
                                15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                15, 15, 15, 15, 15, 15, 15, 15, 15, 15 };
bool isKn[13] = { FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE };
bool isKi[13] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE };
bool isRQ[13] = { FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE };
bool isBQ[13] = { FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE };
bool isPawn[13] = { FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE };