#include "def.h"
#include <cstdlib>

// folosim opratii pe xor pentru a calcula hashKey,
// cheia unica fiecarei poziti, din cheia veche

void hashPiece(int pce, int sq, Board *pos) {
    (pos->hashKey) ^= pieceKeys[pce][sq];
}

void hashCastle(Board *pos) {
    (pos->hashKey) ^= castleKeys[pos->castlePerm];
}

void hashSide(Board *pos) {
    (pos->hashKey) ^= sideKey;
}

void hashEnPas(Board *pos) {
    (pos->hashKey) ^= pieceKeys[EMPTY][pos->enPas];
}

static void clearPiece(int sq, Board *pos) {
    ASSERT(sqOnBoard(sq));
    int pce = pos->pieces[sq];
    ASSERT(pieceValid(pce));
    int i, side = pieceCol[pce];

    hashPiece(pce, sq, pos);
    pos->pieces[sq] = EMPTY;
    pos->material[side] -= pieceVal[pce];

    if (isPieceBig[pce]) {
        pos->bigPcs[side]--;
        if (isPieceMaj[pce])
            pos->majPcs[side]--;
        else
            pos->minPcs[side]--;
    }

    for (i = 0; i < pos->pcsNum[pce]; ++i) {
        if (pos->pList[pce][i] == sq)
            break;
    }
    ASSERT(i != pos->pcsNum[pce]);
    pos->pcsNum[pce]--;
    pos->pList[pce][i] = pos->pList[pce][pos->pcsNum[pce]];
}

static void addPiece(int sq, int pce, Board *pos) {
    ASSERT(sqOnBoard(sq));
    ASSERT(pieceValid(pce));
    int side = pieceCol[pce];
    hashPiece(pce, sq, pos);
    pos->pieces[sq] = pce;

    if (isPieceBig[pce]) {
        pos->bigPcs[side]++;
        if (isPieceMaj[pce])
            pos->majPcs[side]++;
        else
            pos->minPcs[side]++;
    }

    pos->material[side] += pieceVal[pce];
    pos->pList[pce][pos->pcsNum[pce]] = sq;
    pos->pcsNum[pce]++;
}

static void movePiece(int from, int to, Board *pos) {
    ASSERT(sqOnBoard(from));
    ASSERT(sqOnBoard(to));

    int pce = pos->pieces[from];
    int side = pieceCol[pce];
#ifdef DEBUG
    bool validation = false;
#endif
    hashPiece(pce, from, pos);
    pos->pieces[from] = EMPTY;
    hashPiece(pce, to, pos);
    pos->pieces[to] = pce;

    for (int i = 0; i < pos->pcsNum[pce]; i++) {
        if (pos->pList[pce][i] == from) {
            pos->pList[pce][i] = to;
#ifdef DEBUG
            validation = true;
#endif
            break;
        }
    }

    ASSERT(validation);
}

bool makeMove(Move move, Board *pos) {
    ASSERT(checkBoard)
    ASSERT(sqOnBoard(move.from));
    ASSERT(sqOnBoard(move.to));
    ASSERT(sideValid(pos->side));
    ASSERT2(pieceValid(pos->pieces[move.from]), move, pos);

    pos->history[pos->ply].hashKey = pos->hashKey;
    pos->history[pos->hisply].move = move;
    pos->history[pos->hisply].fiftyMoves = pos->fiftyMoves;
    pos->history[pos->hisply].enPas = pos->enPas;
    pos->history[pos->hisply].castlePerm = pos->castlePerm;

    if (move.enPas) {
        if (pos->side == BLACK){
            //am capturat pionul lui white care e cu un rand mai sus

            clearPiece(move.to+10, pos);
        }else{
            //am capturat pionul lui white care e cu un rand mai jos

            clearPiece(move.to-10, pos);
        }
    } else if (move.castle) {
        // daca e rocada trebuie sa mutam si tura
        switch (move.to) {
            case C1: movePiece(A1, D1, pos);
                break;
            case G1: movePiece(H1, F1, pos);
                break;
            case C8: movePiece(A8, D8, pos);
                break;
            case G8: movePiece(H8, F8, pos);
                break;
            default: ASSERT(false);
        }
    }

    if(pos->enPas != NO_SQ)
        hashEnPas(pos);
    hashCastle(pos);

    pos->castlePerm &= castlePerm[move.from];
    pos->castlePerm &= castlePerm[move.to];
    hashCastle(pos);

    pos->fiftyMoves++;
    pos->hisply++;
    pos->ply++;

    if(move.captured){
        ASSERT(pieceValid(move.captured));
        clearPiece(move.to, pos);
        pos->fiftyMoves = 0;
    }

    pos->enPas = NO_SQ;
    if (isPawn[pos->pieces[move.from]]) {
        pos->fiftyMoves = 0;
        if (move.pawnStart) {
            if (pos->side == BLACK) {
                pos->enPas = move.from - 10;
                ASSERT(ranks[pos->enPas] == RANK_6);
            } else {
                pos-> enPas = move.from + 10;
                ASSERT(ranks[pos->enPas] == RANK_3);
            }
            hashEnPas(pos);
        }
    }

    movePiece(move.from, move.to, pos);

    if (move.promotion) {
        ASSERT(pieceValid(move.promotion));
        ASSERT(!isPawn[move.promotion]);
        clearPiece(move.to, pos);
        addPiece(move.to, move.promotion, pos);
    }
    int side = pos->side;
    (pos->side) ^= 1;
    hashSide(pos);
    // TODO 3: undoMove
    if(sqAttacked(pos->pList[wK + 6*side][pos->pcsNum[wK + 6*side] - 1], pos->side, pos)) {
        undoMove(pos);
        return false;
    }
    return true;
}

void undoMove(Board *pos) {
    ASSERT(checkBoard(pos));
    pos->ply--;
    pos->hisply--;
    Move move = pos->history[pos->hisply].move;
    ASSERT(sqOnBoard(move.from));
    ASSERT(sqOnBoard(move.to));

    if(pos->enPas != NO_SQ)
        hashEnPas(pos);
    hashCastle(pos);

    pos->fiftyMoves = pos->history[pos->hisply].fiftyMoves;
    pos->enPas = pos->history[pos->hisply].enPas;
    pos->castlePerm = pos->history[pos->hisply].castlePerm;

    if(pos->enPas != NO_SQ)
        hashEnPas(pos);
    hashCastle(pos);

    (pos->side) ^= 1;
    hashSide(pos);

    if(move.enPas){
        if(pos->side == WHITE)
            addPiece(move.to - 10, bP, pos);
        else
            addPiece(move.to  + 10, wP, pos);
    }else if(move.castle)
        switch(move.to) {
            case C1: movePiece(D1, A1, pos);
                break;
            case C8: movePiece(D8, A8, pos);
                break;
            case G1: movePiece(F1, H1, pos);
                break;
            case G8: movePiece(F8, H8, pos);
                break;
            default: ASSERT(false);
        }

    movePiece(move.to, move.from, pos);

    if(move.captured) {
        ASSERT(pieceValid(move.captured));
        addPiece(move.to, move.captured, pos);
    }

    if(move.promotion)   {
        ASSERT(pieceValid(move.promotion));
        ASSERT(!isPawn[move.promotion]);
        clearPiece(move.from, pos);
        addPiece(move.from, (pieceCol[move.promotion] == WHITE ? wP : bP), pos);
    }

    ASSERT(checkBoard(pos));
}