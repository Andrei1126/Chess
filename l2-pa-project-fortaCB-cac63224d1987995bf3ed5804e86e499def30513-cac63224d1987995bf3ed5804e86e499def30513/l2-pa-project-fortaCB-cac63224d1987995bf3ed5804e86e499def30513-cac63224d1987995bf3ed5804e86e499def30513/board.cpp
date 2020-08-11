
#include <iostream>
#include <string>
#include "def.h"

void resetBoard(Board *pos) {
    int i;

    for (i = 0; i < 120; ++i){
        pos->pieces[i] = OFFBOARD;
    }
    for (i = 0; i < 64; ++i){
        pos->pieces[SQ120(i)] = EMPTY;
    }

    for (i = 0; i < 2; ++i){
        pos->bigPcs[i] = 0;
        pos->majPcs[i] = 0;
        pos->minPcs[i] = 0;
        pos->material[i] = 0;
    }

    for (i = wP; i <= bK; i++) {
        pos->pcsNum[i] = 0;
    }

    pos->side = BOTH;
    pos->enPas = NO_SQ;
    pos->fiftyMoves = 0;
    pos->ply = 0;
    pos->hisply = 0;
    pos->castlePerm = 0;
    pos->hashKey = 0;
}

int parseFen(std::string fen, Board *pos) {
    ASSERT(!fen.empty());
    ASSERT(pos!=NULL);

    int  rank = RANK_8;
    int  file = A;
    int  piece = 0;
    int  count = 0;
    int  i = 0;
    int  sq64 = 0;
    int  sq120 = 0;
    resetBoard(pos);

    for (i = 0; (rank >= RANK_1) && i < fen.size(); i++) {
        count = 1;
        switch (fen[i]) {
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = EMPTY;
                count = fen[i] - '0';
                break;

            case '/':
            case ' ':
                rank--;
                file = A;
                continue;

            default:
                printf("FEN error \n");
                return -1;
        }
        for (int j = 0; j < count; j++) {
            sq64 = rank * 8 + file;
            sq120 = SQ120(sq64);
            if (piece != EMPTY) {
                pos->pieces[sq120] = piece;
            }
            file++;
        }
    }

    ASSERT(fen[i] == 'w' || fen[i] == 'b');
    pos->side = (fen[i] == 'w') ? WHITE : BLACK;
    i += 2;

    for (int j = 0; j < 4; j++) {
        if (fen[i] == ' ') {
            break;
        }
        switch(fen[i]) {
            case 'K': pos->castlePerm |= WKCA; break;
            case 'Q': pos->castlePerm |= WQCA; break;
            case 'k': pos->castlePerm |= BKCA; break;
            case 'q': pos->castlePerm |= BQCA; break;
            default: break;
        }
        i++;
    }
    i++;

    ASSERT(pos->castlePerm>=0 && pos->castlePerm <= 15);
    if (fen[i] != '-') {
        file = fen[i] - 'a';
        rank = fen[i + 1] - '1';

        ASSERT(file >= A && file <= H);
        ASSERT(rank >= RANK_1 && rank <= RANK_8);

        pos->enPas = FR2SQ(file,rank);
    }
    pos->hashKey = generateHashKey(pos);

    updateListsMaterial(pos);
    return 0;
}

char pcsChar[] = ".PNBRQKpnbrqk";
char sideChar[] = "wb-";
char rankChar[] = "12345678";
char fileChar[] = "abcdefgh";
void printBoard(const Board *pos) {
    int sq, file, rank, piece;

    printf("\n");
    for(rank = RANK_8; rank >= RANK_1; rank--) {
        printf("%d  ",rank+1);
        for(file = A; file <= H; file++) {
            sq = FR2SQ(file,rank);
            piece = pos->pieces[sq];
            printf("%3c",pcsChar[piece]);
        }
        printf("\n");
    }

    printf("\n   ");
    for(file = A; file <= H; file++) {
        printf("%3c",'a'+file);
    }

    printf("\n");
    printf("side:%c\n",sideChar[pos->side]);
    printf("enPas:%d\n",pos->enPas);
    printf("castle:%c%c%c%c\n",
           pos->castlePerm & WKCA ? 'K' : '-',
           pos->castlePerm & WQCA ? 'Q' : '-',
           pos->castlePerm & BKCA ? 'k' : '-',
           pos->castlePerm & BQCA ? 'q' : '-');
    printf("PosKey:%llX\n",pos->hashKey);
}

void updateListsMaterial(Board *pos) {
    int piece, colour;
    for (int sq = 0; sq < 120; sq++){
        piece = pos->pieces[sq];
        if(piece!=OFFBOARD && piece!=EMPTY){
            colour = pieceCol[piece];
            if(isPieceBig[piece] == TRUE) pos->bigPcs[colour]++;
            if(isPieceMin[piece] == TRUE) pos->minPcs[colour]++;
            if(isPieceMaj[piece] == TRUE) pos->majPcs[colour]++;

            pos->material[colour] += pieceVal[piece];
            pos->pList[piece][pos->pcsNum[piece]] = sq;
            pos->pcsNum[piece]++;

        }
    }
}

bool checkBoard(Board *pos) {
    int tPcsNum[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int tBigPcs[2] = {0, 0};
    int tMajPcs[2] = {0, 0};
    int tMinPcs[2] = {0, 0};
    int tMaterial[2] = {0, 0};
    int sq64, sq120, tPiece, colour, tNum;

    for(tPiece = wP; tPiece <= bK; tPiece++){
        for(tNum = 0; tNum < pos->pcsNum[tPiece]; tNum++){
            sq120 = pos->pList[tPiece][tNum];
            if (!(pos->pieces[sq120] == tPiece)) {
                return false;
            }
        }
    }
    for(sq64 = 0; sq64 < 64; sq64++){
        sq120 = SQ120(sq64);
        tPiece = pos->pieces[sq120];
        tPcsNum[tPiece]++;
        colour = pieceCol[tPiece];
        if(isPieceBig[tPiece] == TRUE) tBigPcs[colour]++;
        if(isPieceMaj[tPiece] == TRUE) tMajPcs[colour]++;
        if(isPieceMin[tPiece] == TRUE) tMinPcs[colour]++;
        tMaterial[colour] += pieceVal[tPiece];
    }
    for(tPiece = wP; tPiece <= bK; tPiece++){
        if (!(tPcsNum[tPiece] == pos->pcsNum[tPiece]))
            return false;
    }

    if (!(tMaterial[WHITE] == pos->material[WHITE] && tMaterial[BLACK] == pos->material[BLACK]))
        return false;
    if (!(tMinPcs[WHITE] == pos->minPcs[WHITE] && tMinPcs[BLACK] == pos->minPcs[BLACK]))
        return false;
    if (!(tMajPcs[WHITE] == pos->majPcs[WHITE] && tMajPcs[BLACK] == pos->majPcs[BLACK]))
        return false;
    if (!(tBigPcs[WHITE] == pos->bigPcs[WHITE] && tBigPcs[BLACK] == pos->bigPcs[BLACK]))
        return false;
    if (!(pos->side == WHITE || pos->side == BLACK))
        return false;
    if (!(generateHashKey(pos) == pos->hashKey))
        return false;

    return true;
}

bool checkBoard2(Board *pos) {
    int tPcsNum[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int tBigPcs[13] = {0, 0};
    int tMajPcs[13] = {0, 0};
    int tMinPcs[13] = {0, 0};
    int tMaterial[13] = {0, 0};
    int sq64, sq120, tPiece, colour, tNum;

    for(tPiece = wP; tPiece <= bK; tPiece++){
        for(tNum = 0; tNum < pos->pcsNum[tPiece]; tNum++){
            sq120 = pos->pList[tPiece][tNum];
            ASSERT(pos->pieces[sq120] == tPiece);
        }
    }
    for(sq64 = 0; sq64 < 64; sq64++){
        sq120 = SQ120(sq64);
        tPiece = pos->pieces[sq120];
        tPcsNum[tPiece]++;
        colour = pieceCol[tPiece];
        if(isPieceBig[tPiece] == TRUE) tBigPcs[colour]++;
        if(isPieceMaj[tPiece] == TRUE) tMajPcs[colour]++;
        if(isPieceMin[tPiece] == TRUE) tMinPcs[colour]++;
        tMaterial[colour] += pieceVal[tPiece];
    }
    for(tPiece = wP; tPiece <= bK; tPiece++){
        ASSERT(tPcsNum[tPiece] == pos->pcsNum[tPiece]);
    }

    ASSERT(tMaterial[WHITE] == pos->material[WHITE] && tMaterial[BLACK] == pos->material[BLACK]);
    ASSERT(tMinPcs[WHITE] == pos->minPcs[WHITE] && tMinPcs[BLACK] == pos->minPcs[BLACK]);
    ASSERT(tMajPcs[WHITE] == pos->majPcs[WHITE] && tMajPcs[BLACK] == pos->majPcs[BLACK]);
    ASSERT(tBigPcs[WHITE] == pos->bigPcs[WHITE] && tBigPcs[BLACK] == pos->bigPcs[BLACK]);
    ASSERT(pos->side == WHITE || pos->side == BLACK);
    ASSERT(generateHashKey(pos) == pos->hashKey)

    return true;
}
