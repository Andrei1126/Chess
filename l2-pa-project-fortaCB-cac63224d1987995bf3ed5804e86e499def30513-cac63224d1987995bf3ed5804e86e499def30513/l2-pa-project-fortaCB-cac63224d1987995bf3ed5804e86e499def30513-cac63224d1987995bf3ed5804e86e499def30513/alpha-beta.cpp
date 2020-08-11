#include "def.h"
#include <iostream>
#include <string>
using namespace std;

bool threeRep(Board *pos) {
    int r = 0;
    for (int i = pos->ply - pos->fiftyMoves - 1; i < pos->ply - 1; ++i)	{
        if (pos->history[i].hashKey == pos->hashKey)
            r++;
    }
    return r == 2;
}

bool drawMaterial(const Board *pos) {

    if (pos->pcsNum[wP] || pos->pcsNum[bP])
        return false;
    if (pos->pcsNum[wQ] || pos->pcsNum[bQ] || pos->pcsNum[wR] || pos->pcsNum[bR])
        return false;
    if (pos->pcsNum[wB] > 1 || pos->pcsNum[bB] > 1)
        return false;
    if (pos->pcsNum[wN] > 1 || pos->pcsNum[bN] > 1)
        return false;
    if ((pos->pcsNum[wN] && pos->pcsNum[wB]) || (pos->pcsNum[bN] && pos->pcsNum[bB]))
        return false;

    return true;
}

int gameOver(Board *pos) {
/*
 * returns:
-1 white lose
1 black lose
0 tie
2 nothing
*/
    if (pos->fiftyMoves >= 100) {
        return 0;
    }

    if (threeRep(pos)) {
        return 0;
    }

    if (drawMaterial(pos)) {
        return 0;
    }

    vector<Move> list;
    generateAllMoves(pos, list);

    bool foundMove = false;
    for(Move move : list) {
        if (makeMove(move, pos)) {
            foundMove = true;
            undoMove(pos);
            break;
        }
    }

    if(foundMove)
        return 2;
    int pce = wK + 6*pos->side;
    bool inCheck = sqAttacked(pos->pList[pce][pos->pcsNum[pce]-1], pos->side^1, pos);

    if(inCheck)	{
        if(pos->side == WHITE) {
            return -1;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}
// a fost
Move negamax(int depth, Board *pos, Move m) {
    ASSERT(checkBoard(pos));

    int gameResult=gameOver(pos);
    if (depth == 0 && gameResult==2) {
        m.score = evalPosition(pos);
        return m;
    }else{
        if(gameResult==0){
            m.score = 0;
            return m;
        }
        if(gameResult == -1 && pos->side==BLACK){
            m.score = 10000;
            return m;
        }
        if(gameResult == 1 && pos->side==WHITE){
            m.score = 10000;
            return m;
        }
        if(gameResult == -1 && pos->side==WHITE){
            m.score = -10000;
            return m;
        }
        if(gameResult == 1 && pos->side==BLACK){
            m.score = -10000;
            return m;
        }
    }

    std::vector<Move> list;
    generateAllMoves(pos,list);

    Move max = list[0];
    max.score = -1e9;
    Move move;
    for (int i = 0; i < list.size(); i++) {
        move = list[i];
        if (!makeMove(move, pos))
            continue;

        Move result = negamax(depth-1, pos, move);

        result.score *= -1;

        if (max.score < result.score) {
            max = move;
            max.score = result.score;
        }

        undoMove(pos);
    }
    return max;
}

// am folosit modeul din laborator
Move alphaBetaNegamax(int alpha, int beta, int depth, Board *pos, Move m) {
    ASSERT(checkBoard2(pos));
    int gameResult=gameOver(pos);
    if (depth == 0 && gameResult==2) {
        m.score = evalPosition(pos);
        return m;
    }else{
        if(gameResult==0){
            m.score = 0;
            return m;
        }
        if(gameResult == -1 && pos->side==BLACK){
            m.score = 10000;
            return m;
        }
        if(gameResult == 1 && pos->side==WHITE){
            m.score = 10000;
            return m;
        }
        if(gameResult == -1 && pos->side==WHITE){
            m.score = -10000;
            return m;
        }
        if(gameResult == 1 && pos->side==BLACK){
            m.score = -10000;
            return m;
        }
    }

    std::vector<Move> list;
    generateAllMoves(pos,list);

    Move max = list[0];
    max.score = -1e9;
    Move move;
    for (int i = 0; i < list.size(); i++) {
        move = list[i];
        if (!makeMove(move, pos))
            continue;

        Move result = alphaBetaNegamax(-beta, -alpha, depth-1, pos, move);

        result.score *= -1;
        // o miscare catigatoare este cu atat mai valoroasa cu cat aduce victoria mia repede
        // cu cate mai in spate in arborele de cautare cu atat ii scadem scorul mai mult
        if(result.score > 5000){
            result.score = 5000 + (result.score - 5000) * 80 / 100;
        }
        undoMove(pos);

        if (max.score < result.score) {
            max = move;
            max.score = result.score;
        }
        if (max.score > alpha)
            alpha = max.score;

        if (alpha >= beta) {
            break;
        }
    }
    return max;
}
