
#ifndef DEF_H
#define DEF_H
#include <iostream>
#include <string>
#include <vector>
typedef unsigned long long U64;

enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK  };
enum { A, B, C, D, E, F, G, H, NONE };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };
enum { WHITE, BLACK, BOTH };
enum { FALSE, TRUE };
enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };
enum {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
};

//#define DEBUG

// file-rank to square in 120 array
#define FR2SQ(f,r) ( (21 + (f)) + ((r) * 10) )
#define SQ64(sq120) (Sq120ToSq64[(sq120)])
#define SQ120(sq64) (Sq64ToSq120[(sq64)])
#define NOMOVE (new Move(0,0,0,0))

class Move {
public:

    int from{};
    int to{};
    int captured{};
    bool enPas{};
    int promotion{};
    bool pawnStart{};
    bool castle{};
    int score{};

    Move () {
        from = OFFBOARD;
        to =  OFFBOARD;
        captured = EMPTY;
        promotion = EMPTY;
        enPas = false;
        pawnStart = false;
        castle = false;
        score = 0;
    }

    Move(int from, int to, int captured, int promotion) {
        this->from = from;
        this->to = to;
        this->captured = captured;
        this->promotion = promotion;
        enPas = false;
        castle = false;
        pawnStart = false;
        score = 0;
    }

    Move(int from, int to, int captured, int promotion, bool castle, bool pawnStart, bool enPas) {
        this->from = from;
        this->to = to;
        this->captured = captured;
        this->promotion = promotion;
        this->castle = castle;
        this->pawnStart = pawnStart;
        this->enPas = enPas;
        score = 0;
    }

    bool compareTo(Move *move) {
        return this->from == move->from && this->to == move->to
                && this->promotion == move->promotion;
    }
};


class Undo {
public:
    Move move;
    int castlePerm;
    int enPas;
    int fiftyMoves;
    U64 hashKey;
};

class Board {
public:
    // 100  100  100  100  100  100  100  100  100  100
    // 100  100  100  100  100  100  100  100  100  100
    // 100   63   62   61   60   59   58   57   56  100
    // 100   55   54   53   52   51   50   49   48  100
    // 100   47   46   45   44   43   42   41   40  100
    // 100   39   38   37   36   35   34   33   32  100
    // 100   31   30   29   28   27   26   25   24  100
    // 100   23   22   21   20   19   18   17   16  100
    // 100   15   14   13   12   11   10    9    8  100
    // 100    7    6    5    4    3    2    1    0  100
    // 100  100  100  100  100  100  100  100  100  100
    // 100  100  100  100  100  100  100  100  100  100
    int pieces[120];

    // 0 alb
    // 1 negru
    int side;
    int enPas;

    // incepe de la 0 cand ajunge la 50 -> egalitate
    // se reseteaza la avansarea unui pion sau la captura
    int fiftyMoves;

    // 1 white king side
    // 2 white queen side
    // 4 black king side
    // 8 black queen side
    int castlePerm;

    // numara mutarile (jumatati de mutari)
    int ply;
    int hisply;
    U64 hashKey;

    // trebuiau folosite intr o funtie de evaluare mai eficienta
    // dar nu am apucat sa o terminam

    // pcsNum[i] = nr de piese i
    int pcsNum[13];
    // bigPcs[side] = nr de piese in afara de pion
    int bigPcs[2];
    // major piece = nr de queen, rook, king
    int majPcs[2];
    // minor piece = nr de bishop, knight
    int minPcs[2];
    // valoarea tuturor pieselor adunate
    int material[2];
    // p[i][j] -> pozitia lui piesei i este j
    int pList[13][10];

    Undo history[1024];
};

extern int Sq120ToSq64[120];
extern int Sq64ToSq120[64];
extern U64 pieceKeys[13][120];
extern U64 sideKey;
extern U64 castleKeys[16];
extern const int KnDir[8];
extern const int RkDir[4];
extern const int BiDir[4];
extern const int KiDir[8];
extern int pieceVal[13];
extern bool isPieceBig[13];
extern bool isPieceMaj[13];
extern bool isPieceMin[13];
extern int pieceCol[13];
extern const int castlePerm[120];
extern bool isKn[13];
extern bool isKi[13];
extern bool isRQ[13];
extern bool isBQ[13];
extern bool isPawn[13];
extern int files[120];
extern int ranks[120];

extern U64 generateHashKey(Board *pos);
extern void allInit();
extern void updateListsMaterial(Board *pos);
extern void resetBoard(Board *pos);
extern int sqAttacked(int sq, int side, Board *pos);
extern bool makeMove(Move move, Board *pos);
extern void undoMove(Board *pos);
extern int parseFen(std::string fen, Board *pos);
extern void generateAllMoves(Board *pos, std::vector<Move> &allMoves);
extern int evalPosition(Board *pos);
extern Move negamax(int depth, Board *pos, Move m);
extern Move alphaBetaNegamax(int alpha, int beta, int depth, Board *pos, Move m);

// functii de afisare
extern char *prMove(Move move);

// validare
extern bool sqOnBoard(int sq);
extern bool sideValid(int side);
extern bool pieceValid(int pce);
extern bool checkBoard(Board *pos);
extern bool checkBoard2(Board *pos);
extern int gameOver(Board *pos);


#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed", #n); \
printf("At %s ",__TIME__); \
printf("In File %s ",__FILE__); \
printf("At Line %d\n",__LINE__); \
exit(1);}
#endif

#ifndef DEBUG
#define ASSERT2(n, move, b)
#else
#define ASSERT2(n, move, b) \
if(!(n)) { \
printf("%s, - Failed\n", #n); \
printMove(move); \
printBoard(b); \
printf("\nAt %s ",__TIME__); \
printf("In File %s ",__FILE__); \
printf("At Line %d\n",__LINE__); \
exit(1);}
#endif


#endif //DEF_H