#include "def.h"
#include <vector>
using namespace std;

// ne folosimde acesti vectori pentru a intra prin diferitele tipuri de piese
// cele care se pot nuta pe o directie pe mai multe pozitii: slidePcs
// si cele care nu: nonSlidePcs
const int slidePcs[8] = {wB, wR, wQ, 0, bB, bR, bQ, 0};
const int nonSlidePcs[6] = {wN, wK, 0, bN, bK, 0};
const int numOfDir[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};
const int pceDir[13][8] = { { 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0 },
                            { -8, -19,	-21, -12, 8, 19, 21, 12 },
                            { -9, -11, 11, 9, 0, 0, 0, 0 },
                            { -1, -10,	1, 10, 0, 0, 0, 0 },
                            { -1, -10,	1, 10, -9, -11, 11, 9 },
                            { -1, -10,	1, 10, -9, -11, 11, 9 },
                            { 0, 0, 0, 0, 0, 0, 0 },
                            { -8, -19,	-21, -12, 8, 19, 21, 12 },
                            { -9, -11, 11, 9, 0, 0, 0, 0 },
                            { -1, -10,	1, 10, 0, 0, 0, 0 },
                            { -1, -10,	1, 10, -9, -11, 11, 9 },
                            { -1, -10,	1, 10, -9, -11, 11, 9 } };

void generateAllMoves(Board *pos, vector<Move> &allMoves) {
    ASSERT(checkBoard(pos));
    int pceNum, sq, to, i, j, pce = EMPTY, side =pos->side, dir;
    int left = 9, right = 11, forward = 10, rank = RANK_2;

    // rocada
    if (side == BLACK){
        left = -9;
        right = -11;
        forward = -10;
        rank = RANK_7;

        if (pos->castlePerm & BKCA)
            if (pos->pieces[F8] == EMPTY && pos->pieces[G8] == EMPTY)
                if (!sqAttacked(E8, WHITE, pos) && !sqAttacked(G8, WHITE, pos) && !sqAttacked(F8, WHITE, pos))
                    allMoves.emplace_back(E8, G8, EMPTY, EMPTY, true, false, false);
        if (pos->castlePerm & BQCA)
            if (pos->pieces[D8] == EMPTY && pos->pieces[C8] == EMPTY && pos->pieces[B8] == EMPTY)
                if (!sqAttacked(E8, WHITE, pos) && !sqAttacked(C8, WHITE, pos) && !sqAttacked(D8,WHITE, pos))
                    allMoves.emplace_back(E8, C8, EMPTY, EMPTY, true, false, false);
    } else {
        if (pos->castlePerm & WKCA)
            if (pos->pieces[F1] == EMPTY && pos->pieces[G1] == EMPTY)
                if (!sqAttacked(E1, BLACK, pos) && !sqAttacked(G1, BLACK, pos) && !sqAttacked(F1,BLACK, pos))
                    allMoves.emplace_back(E1, G1, EMPTY, EMPTY, true,  false, false);
        if (pos->castlePerm & WQCA)
            if (pos->pieces[D1] == EMPTY && pos->pieces[C1] == EMPTY && pos->pieces[B1] == EMPTY)
                if (!sqAttacked(E1, BLACK, pos) && !sqAttacked(C1, BLACK, pos) && !sqAttacked(D1, BLACK, pos))
                    allMoves.emplace_back(E1, C1, EMPTY, EMPTY, true,  false, false);
    }
    
    i = 4*side;
    pce = slidePcs[i++];
    while(pce){
        ASSERT(pieceValid(pce));

        for(pceNum = 0; pceNum < pos->pcsNum[pce]; pceNum++){
            sq = pos->pList[pce][pceNum];
            ASSERT(sqOnBoard(sq));

            for(j = 0; j < numOfDir[pce]; j++){
                dir = pceDir[pce][j];
                to = sq + dir;

                while(files[to] != OFFBOARD){
                    if(pos->pieces[to] != EMPTY){
                        if(pieceCol[pos->pieces[to]] == (side^1))
                            allMoves.emplace_back(sq, to, pos->pieces[to], EMPTY);
                        break;
                    }
                    allMoves.emplace_back(sq, to,EMPTY, EMPTY);
                    to += dir;
                }
            }
        }
        pce = slidePcs[i++];
    }

    i = 3*side;
    pce = nonSlidePcs[i++];
    while(pce){
        ASSERT(pieceValid(pce));

        for(pceNum = 0; pceNum < pos->pcsNum[pce]; pceNum++){
            sq = pos->pList[pce][pceNum];
            ASSERT(sqOnBoard(sq));

            for(j = 0; j < numOfDir[pce]; j++){
                dir = pceDir[pce][j];
                to = sq + dir;

                if (files[to] == OFFBOARD)
                    continue;
                if (isKi[pce] && sqAttacked(to, side^1, pos))
                    continue;
                if(pos->pieces[to] != EMPTY){
                    if(pieceCol[pos->pieces[to]] == (side^1))
                        allMoves.emplace_back(sq, to, pos->pieces[to], EMPTY);
                    continue;
                }
                allMoves.emplace_back(sq, to, EMPTY, EMPTY);
            }
        }
        pce = nonSlidePcs[i++];
    }
    // pioni
    for(pceNum = 0; pceNum < pos->pcsNum[wP+6*side]; pceNum++) {
        sq = pos->pList[wP+6*side][pceNum];
        ASSERT(sqOnBoard(sq));

        if(ranks[sq + forward] != 7*(side^1)) {
            if (pos->pieces[sq + forward] == EMPTY) {
                allMoves.emplace_back(sq, sq + forward, EMPTY, EMPTY);
                if (ranks[sq] == rank && pos->pieces[sq + 2 * forward] == EMPTY)
                    allMoves.emplace_back(sq, sq + 2 * forward, EMPTY, EMPTY, false, true, false);
            }
            if (files[sq + left] != OFFBOARD && pieceCol[pos->pieces[sq + left]] == (side ^ 1))
                allMoves.emplace_back(sq, sq + left, pos->pieces[sq + left], EMPTY, false, false, false);

            if (files[sq + right] != OFFBOARD && pieceCol[pos->pieces[sq + right]] == (side ^ 1))
                allMoves.emplace_back(sq, sq + right, pos->pieces[sq + right], EMPTY);
            if (sq + left == pos->enPas)
                allMoves.emplace_back(sq, sq + left, EMPTY, EMPTY);
            if (sq + right == pos->enPas)
                allMoves.emplace_back(sq, sq + right, EMPTY, EMPTY);
        } else {
            if (pos->pieces[sq + forward] == EMPTY) {
                allMoves.emplace_back(sq, sq + forward, EMPTY, side == BLACK ? bQ : wQ);
                allMoves.emplace_back(sq, sq + forward, EMPTY, side == BLACK ? bR : wR);
                allMoves.emplace_back(sq, sq + forward, EMPTY, side == BLACK ? bB : wB);
                allMoves.emplace_back(sq, sq + forward, EMPTY, side == BLACK ? bN : wN);
            }
            if (files[sq + left] != OFFBOARD && pieceCol[pos->pieces[sq + left]] == (side ^ 1)) {
                allMoves.emplace_back(sq, sq + left, pos->pieces[sq + left], side == BLACK ? bQ : wQ, false, false, false);
                allMoves.emplace_back(sq, sq + left, pos->pieces[sq + left], side == BLACK ? bR : wR, false, false, false);
                allMoves.emplace_back(sq, sq + left, pos->pieces[sq + left], side == BLACK ? bB : wB, false, false, false);
                allMoves.emplace_back(sq, sq + left, pos->pieces[sq + left], side == BLACK ? bN : wN, false, false, false);
            }
            if (files[sq + right] != OFFBOARD && pieceCol[pos->pieces[sq + right]] == (side ^ 1)) {
                allMoves.emplace_back(sq, sq + right, pos->pieces[sq + right], side == BLACK ? bQ : wQ);
                allMoves.emplace_back(sq, sq + right, pos->pieces[sq + right], side == BLACK ? bR : wR);
                allMoves.emplace_back(sq, sq + right, pos->pieces[sq + right], side == BLACK ? bB : wB);
                allMoves.emplace_back(sq, sq + right, pos->pieces[sq + right], side == BLACK ? bN : wN);
            }
        }
    }
}