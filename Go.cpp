#include "Go.hpp"
#include <string> //string, stoi
#include <cctype> //toupper, tolower
#include <iomanip> //put_time
#include <sstream> //ostringstream
#include <fstream> //ofstream
#include <chrono> //system_clock
#include <ctime> //ctime()
#include <iostream>

Go::Go() : Go("Black", "White") {}

Go::Go(const std::string& name1, const std::string& name2) : board{}, captures{}, player1{name1}, player2{name2}, gameOver{false} {}

Stone Go::stone(const int row, const int col) const {
    return board[row * BOARDSIZE + col];
}

std::string Go::sgfToStd(const std::string& sgfMove){
    int c = int(sgfMove[2]);
    int r = int(sgfMove[3]);
    return std::string{sgfMove[0], ' ', (c < 105 ? (char)std::toupper(c) : (char)std::toupper(c+1))} + std::to_string(116-r);
}

void Go::saveSgf() const{
    saveSgf(".");
}

void Go::saveSgf(const std::string& sgfDir) const {
    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::ostringstream filename;
    filename << sgfDir << '/';
    filename << player1 << "_vs_" << player2 << '_';
    filename << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S");
    std::ofstream file{filename.str() + ".sgf"};
    file << "(;FF[4]\nCA[UTF-8]\nGM[1]\nGN[" << filename.str().substr(sgfDir.size()+1,std::string::npos) << "]\nAP[Ufuk's Go app]\nPB["<< player1 << "]\nPW[" << player2 << "]\n";
    file << "SZ[" << BOARDSIZE << "]\nKM[" << KOMI << "]\nRU[Chinese]\nDT[" << std::put_time(std::localtime(&t), "%Y-%m-%d") << "]\n";
    for(std::string move : moves){
        file << ';' << move << '\n';
    }
    file << ')';
    file.close();
}

std::string Go::toString() const {
    return toString('+', 'B', 'W');
}

std::string Go::toString(const char empty, const char black, const char white) const {
    std::ostringstream res;
    for(int i = 0; i < BOARDSIZE*BOARDSIZE; i++){
        switch (board[i]){
            case Stone::Empty:
                res << empty;
                break;
            case Stone::Black:
                res << black;
                break;
            case Stone::White:
                res << white;
                break;
        }
        if ((i+1) % BOARDSIZE == 0)
            res << "\n";
    }
    return res.str();
}

bool Go::play(std::string stdMove){
    Stone s = Stone((moves.size() % 2) + 1); //stone to be played
    //Convert stdMove('A-T'+'1-19' with i excluded) -> move(row, col)
    int col = std::tolower(stdMove[0]) - 97;
    if (col >= 8) //'i' ascii -> 105 - 97 = 8
        col -= 1;
    stdMove.erase(0, 1);
    int row = 19 - std::stoi(stdMove);
    makeMove(row, col, s);
    return true;
}

bool Go::makeMove(const int row, const int col, const Stone s){
    if(gameOver)
        return false;
    moves.push_back(std::string{(s==Stone::Black ? 'B' : 'W'), '[', char(col + 97), char(row + 97),']'}); //Convert move(row, col, s) -> sgf move
    Stone& cs = board[row * BOARDSIZE + col];
    if (cs == Stone::Empty)
        cs = s;
    return true;
}