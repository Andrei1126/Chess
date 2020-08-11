#include "def.h"

// folosim vectorii pentru calcula cat valoreata fiecare piesa din pozitia curenta
// pe fiecare pozitie
// au fost luati de pe:
// https://github.com/bluefeversoft/Vice_Chess_Engine
const int PawnTable[64] = {
        0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
        10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
        5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
        0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
        5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
        10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
        20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
        0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int KnightTable[64] = {
        0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
        0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
        0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
        0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
        5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
        5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int BishopTable[64] = {
        0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
        0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
        0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
        0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
        0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
        0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
        0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
        0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int RookTable[64] = {
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0
};

const int Mirror64[64] = {
        56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
        48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
        40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
        32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
        24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
        16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
        8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
        0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};


bool sqOnBoard(const int sq){
    return files[sq] != OFFBOARD;
}

bool sideValid(const int side){
    return (side==WHITE || side == BLACK);
}

bool fileRankValid(const int fr){
    return (fr >= 0 && fr <= 7);
}

bool pieceValidOrEmpty(const int pce){
    return (pce >= EMPTY && pce <= bK);
}

bool pieceValid(const int pce){
    return (pce >= wP && pce <= bK);
}

int evalPosition(Board *pos) {

    int pce;
    int pceNum;
    int sq;
    int score = pos->material[WHITE] - pos->material[BLACK];

    // dun cauza ca nu ne am organizat foarte bine cu timpul
    // am facut o functie de evalare foarte simpla
    pce = wP;
    for(pceNum = 0; pceNum < pos->pcsNum[pce]; ++pceNum) {
        sq = pos->pList[pce][pceNum];
        ASSERT(sqOnBoard(sq));
        score += PawnTable[SQ64(sq)];
    }

    pce = bP;
    for(pceNum = 0; pceNum < pos->pcsNum[pce]; ++pceNum) {
        sq = pos->pList[pce][pceNum];
        ASSERT(sqOnBoard(sq));
        score -= PawnTable[Mirror64[SQ64(sq)]];
    }

    pce = wN;
    for(pceNum = 0; pceNum < pos->pcsNum[pce]; ++pceNum) {
        sq = pos->pList[pce][pceNum];
        ASSERT(sqOnBoard(sq));
        score += KnightTable[SQ64(sq)];
    }

    pce = bN;
    for(pceNum = 0; pceNum < pos->pcsNum[pce]; ++pceNum) {
        sq = pos->pList[pce][pceNum];
        ASSERT(sqOnBoard(sq));
        score -= KnightTable[Mirror64[SQ64(sq)]];
    }

    pce = wB;
    for(pceNum = 0; pceNum < pos->pcsNum[pce]; ++pceNum) {
        sq = pos->pList[pce][pceNum];
        ASSERT(sqOnBoard(sq));
        score += BishopTable[SQ64(sq)];
    }

    pce = bB;
    for(pceNum = 0; pceNum < pos->pcsNum[pce]; ++pceNum) {
        sq = pos->pList[pce][pceNum];
        ASSERT(sqOnBoard(sq));
        score -= BishopTable[Mirror64[SQ64(sq)]];
    }

    pce = wR;
    for(pceNum = 0; pceNum < pos->pcsNum[pce]; ++pceNum) {
        sq = pos->pList[pce][pceNum];
        ASSERT(sqOnBoard(sq));
        score += RookTable[SQ64(sq)];
    }

    pce = bR;
    for(pceNum = 0; pceNum < pos->pcsNum[pce]; ++pceNum) {
        sq = pos->pList[pce][pceNum];
        ASSERT(sqOnBoard(sq));
        score -= RookTable[Mirror64[SQ64(sq)]];
    }

    pce = wQ;
    for(pceNum = 0; pceNum < pos->pcsNum[pce]; ++pceNum) {
        sq = pos->pList[pce][pceNum];
        ASSERT(sqOnBoard(sq));
        score += BishopTable[SQ64(sq)];
        score += RookTable[SQ64(sq)];
    }

    pce = bQ;
    for(pceNum = 0; pceNum < pos->pcsNum[pce]; ++pceNum) {
        sq = pos->pList[pce][pceNum];
        ASSERT(sqOnBoard(sq));
        score -= BishopTable[Mirror64[SQ64(sq)]];
        score -= RookTable[Mirror64[SQ64(sq)]];
    }

    if(pos->side == WHITE) {
        return score;
    } else {
        return -score;
    }
}