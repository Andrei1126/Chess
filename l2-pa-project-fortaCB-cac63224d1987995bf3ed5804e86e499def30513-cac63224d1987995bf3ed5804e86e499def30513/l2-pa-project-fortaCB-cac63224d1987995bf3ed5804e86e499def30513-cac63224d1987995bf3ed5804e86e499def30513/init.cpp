
// doar functii necesare generarii hashKey - unica pt fiecare pozite
#include "def.h"
#include <cstdlib>
#define RANDOM_64 ((U64)rand() | (U64)rand() << 15 | (U64)rand() << 30 |\
				 (U64)rand() << 45 | ((U64)rand() & 0xf) << 60)

int Sq120ToSq64[120];
int Sq64ToSq120[64];
int files[120];
int ranks[120];
U64 pieceKeys[13][120];
U64 sideKey;
U64 castleKeys[16];

void InitFilesRasnks(){
    int i, sq = A1, sq64 = 0;
    for (i = 0; i < 120; ++i){
        files[i] = OFFBOARD;
        ranks[i] = OFFBOARD;
    }
    for (int rank = RANK_1; rank <= RANK_8; rank++){
        for (int file = A; file <= H; file++){
            sq = FR2SQ(file, rank);
            files[sq] = file;
            ranks[sq] = rank;
        }
    }
}

void InitHashKeys() {
    int index;
    for(index = 0; index < 13; ++index) {
        for(int index2 = 0; index2 < 120; ++index2) {
            pieceKeys[index][index2] = RANDOM_64;
        }
    }
    sideKey = RANDOM_64;
    for(index = 0; index < 16; ++index) {
        castleKeys[index] = RANDOM_64;
    }
}

void InitSq120To64() {

    int index = 0;
    int file = A;
    int rank = RANK_1;
    int sq = A1;
    int sq64 = 0;
    for(index = 0; index < 120; ++index) {
        Sq120ToSq64[index] = 100;
    }

    for(index = 0; index < 64; ++index) {
        Sq64ToSq120[index] = 120;
    }

    for(rank = RANK_1; rank <= RANK_8; ++rank) {
        for(file = A; file <= H; ++file) {
            sq = FR2SQ(file,rank);
            Sq64ToSq120[sq64] = sq;
            Sq120ToSq64[sq] = sq64;
            sq64++;
        }
    }
}

U64 generateHashKey(Board *pos) {

    int sq = 0;
    U64 finalKey = 0;
    int piece = EMPTY;

    for(sq = 0; sq < 120; ++sq) {
        piece = pos->pieces[sq];
        if(piece!=NO_SQ && piece!=EMPTY && piece != OFFBOARD) {
            ASSERT(piece>=wP && piece<=bK);
            finalKey ^= pieceKeys[piece][sq];
        }
    }

    if(pos->side == WHITE) {
        finalKey ^= sideKey;
    }

    if(pos->enPas != NO_SQ) {
        ASSERT(pos->enPas >= 0 && pos->enPas < 120);
        finalKey ^= pieceKeys[EMPTY][pos->enPas];
    }

    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    finalKey ^= castleKeys[pos->castlePerm];

    return finalKey;
}

void allInit() {
    InitSq120To64();
    InitHashKeys();
    InitFilesRasnks();
}