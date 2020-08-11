#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "def.h"
using namespace std;

#define START_FEN  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

bool isMove(string s) {
    if (s.length() > 5)
        return false;
    if (s[0] < 'a' || s[0] > 'h')
        return false;
    if (s[1] < '1' || s[1] > '8')
        return false;
    if (s[2] < 'a' || s[2] > 'h')
        return false;
    return !(s[3] < '1' || s[3] > '8');
}

int parseTime(string line) {
    stringstream sin(line);
    int time = -1;
    string s;
    sin >> s >> time;
    return time;
}

Move parseMove(string s, Board *b);

void writeMove(Board *pos,int time){
	Move m;
	if(pos->ply == 0 && pos->side==WHITE){
			cout<<"move e2e4"<<endl;
			makeMove(parseMove("e2e4",pos),pos);
			return;
	}
	if(pos->ply == 1 && pos->side==BLACK){
			cout<<"move e7e5"<<endl;
			makeMove(parseMove("e7e5",pos),pos);
			return;
	}
	int depth=4;
	int i,sum=0;
	for(i=wP;i<=bK;i++){
		sum+=pos->pcsNum[i];
	}
	if(time>12000 && pos->ply>30)
		depth=5;
	if(sum<20)
		depth=5;
	if(sum<10)
		depth=6;
	if(time<3000)
		depth--;

	m = alphaBetaNegamax(-1e9,1e9, depth, pos, *NOMOVE);
    if (m.compareTo(NOMOVE) || m.score ==- 10000) {
            cout << "resign" <<endl;
    } else {
            makeMove(m, pos);
            cout<<"move "<<prMove(m)<<endl;
    }
}

int main() {
    Board *b = new Board;
    allInit();
    parseFen(START_FEN, b);
    int mode, time, otime;
    string line;
    Move m;

    while (true) {
        getline(cin, line);
        if (line.find("post") != string::npos)
            continue;
        if (line.find("hard") != string::npos)
            continue;
        if (line.find("easy") != string::npos)
            continue;
        if (line.find("undo") != string::npos)
            continue;
        if (line.find("time") != string::npos) {
            time = parseTime(line);
            continue;
        }
        if (line.find("otim") != string::npos) {
            otime = parseTime(line);
            continue;
        }

        // dezactivam modul care trimite SIGINT SIGTERM SAN
        if (line.find("protover") != string :: npos) {
            cout << "feature sigint=0" << endl;
            cout << "feature san=0" << endl;
            cout << "feature sigterm=0" << endl;
        }

        if (line.find("new") != string :: npos) {
            parseFen(START_FEN, b);
            mode = 1;  // black
            continue;
        }

        // setam modurile
        if (line.find("black") != string :: npos) {
            mode = 1;  // black
            getline(cin, line);
        }
        if (line.find("white") != string :: npos) {
            mode = 0; // white
            getline(cin, line);
        }

        if (line.find("go") != string :: npos) {
            writeMove(b,time);
            continue;
        }

        if (isMove(line)) {
            Move move = parseMove(line, b);
            makeMove(move, b);
           	writeMove(b,time);
            continue;
        }

        if (line.find("force") != string :: npos) { // daca intram in modul force engine ul nu mai face mutari singur
            while (true) {
                getline(cin, line);

                if (line.find("black") != string :: npos) { // asteptam "go" sau "new" ca sa iesim din
                    // force mod
                    mode = 1;  //black
                    break;
                }
                if (line.find("white") != string :: npos) { // asteptam "go" sau "new" ca sa iesim din
                    // force mod
                    mode = 0;  // white
                    break;
                }
                if (line.find("go") != string :: npos){
                    m = alphaBetaNegamax(-1e9,1e9, 4, b, *NOMOVE);
                    writeMove(b,time);
                    break;
                }
                if (line.find("new") != string :: npos) {
                    parseFen(START_FEN, b);
                    mode = 1;  // black
                    break;
                }

                // ignoram comenzile care nu ne intereseaza
                if (line.find("post") != string :: npos)
                    continue;
                if (line.find("hard") != string :: npos)
                    continue;
                if (line.find("easy") != string :: npos)
                    continue;
                if (isMove(line)) {
                    Move move = parseMove(line, b);
                    makeMove(move, b);
                    continue;
                }
            }
        }

        if (line.find("quit") != string :: npos)
            break;
    }
}

Move parseMove(string s, Board *b) {
    if (!isMove(s))
        return *NOMOVE;

    int fileFrom = s[0] - 'a';
    int rankFrom = s[1] - '1';
    int fileTo = s[2] - 'a';
    int rankTo = s[3] - '1';
    int from = FR2SQ(fileFrom, rankFrom);
    int to = FR2SQ(fileTo, rankTo);
    int captured = b->pieces[to]; // daca e ceva ok bine, daca nu va fi EMPTY adica 0

    int promoted = EMPTY;
    if (s.size() == 5) {
        switch (s[4]) {
            case 'q': promoted = b->side == WHITE ? wQ : bQ;
                break;
            case 'r': promoted = b->side == WHITE ? wR : bR;
                break;
            case 'b': promoted = b->side == WHITE ? wB : bB;
                break;
            case 'k': promoted = b->side == WHITE ? wK : bK;
                break;
            default: return *NOMOVE;
        }
    }

    bool castle = false;
    if (b->side == BLACK){

        if (b->castlePerm & BKCA)
            if (b->pieces[F8] == EMPTY && b->pieces[G8] == EMPTY)
                if (!sqAttacked(E8, WHITE, b) && !sqAttacked(F8, WHITE, b))
                    castle = true;
        if (b->castlePerm & BQCA)
            if (b->pieces[D8] == EMPTY && b->pieces[C8] == EMPTY && b->pieces[B8] == EMPTY)
                if (!sqAttacked(E8, WHITE, b) && !sqAttacked(D8, WHITE, b))
                    castle = true;
    } else {
        if (b->castlePerm & WKCA)
            if (b->pieces[F1] == EMPTY && b->pieces[G1] == EMPTY)
                if (!sqAttacked(E1, BLACK, b) && !sqAttacked(F1, BLACK, b))
                    castle = true;
        if (b->castlePerm & WQCA)
            if (b->pieces[D1] == EMPTY && b->pieces[C1] == EMPTY && b->pieces[B1] == EMPTY)
                if (!sqAttacked(E1, BLACK, b) && !sqAttacked(D1, BLACK, b))
                    castle = true;
    }

    bool pawnStart = false;
    if (isPawn[b->pieces[from]]) {
        if (b->side == WHITE && ranks[from] == RANK_2 && ranks[to] == RANK_4) {
            pawnStart = true;
        }
        if (b->side == BLACK && ranks[from] == RANK_7 && ranks[to] == RANK_5) {;
            pawnStart = true;
        }
    }

    bool enPas = false;
    if(b->enPas == to)
        enPas = true;

    Move *m = new Move(from, to, captured, promoted, castle, pawnStart, enPas);
    return *m;
}