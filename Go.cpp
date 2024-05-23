#include "Go.hpp"
#include <string>
#include <cctype> //toupper
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

void Go::saveSgf(const std::string& sgfDir) const {
    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::ostringstream filename;
    filename << player1 << "_vs_" << player2 << '_';
    filename << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S");
    std::cout << filename.str() << std::endl;
    //std::string filename{std::ctime(&t)};
}