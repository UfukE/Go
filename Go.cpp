#include "Go.hpp"
#include <string>
#include <cctype> //toupper

Go::Go() : board{} {}

Player Go::player(const int row, const int col) const {
    return board[row * BOARDSIZE + col];
}

std::string Go::sgfToStd(const std::string& sgfMove){
    int c = int(sgfMove[2]);
    int r = int(sgfMove[3]);
    return std::string{sgfMove[0], ' ', (c < 105 ? (char)std::toupper(c) : (char)std::toupper(c+1))} + std::to_string(116-r);
}

/*std::string Go::stdToSgf(const std::string& stdMove){

}*/