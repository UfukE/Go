#include "Go.hpp"
#include <string> //string, stoi
#include <cctype> //toupper, tolower
#include <iomanip> //put_time
#include <sstream> //ostringstream
#include <fstream> //ofstream, ifstream
#include <chrono> //system_clock
#include <ctime> //ctime()
#include <algorithm> //find
#include <iostream>

Go::Go() : Go("Black", "White") {}

Go::Go(const std::string& name1, const std::string& name2) : board{}, captures{}, player1{name1}, player2{name2}, metaData{"(;FF[4]\nCA[UTF-8]\nGM[1]\nAP[Ufuk's Go app]\nRU[Chinese]\n"}, gameOver{false} {
    metaData += ("PB[" + name1 + "]\nPW[" + name2 + "]\n");
    metaData += ("SZ[" + std::to_string(BOARDSIZE) + "]\nKM[" + std::to_string(HANDICAP > 0 ? -0.5 : KOMI).substr(0, (HANDICAP > 0 ? 4 : 3)) + "]\n");
    setHandi(HANDICAP);
    setHandiMeta(HANDICAP);
}

inline Stone Go::stone(const int row, const int col) const {
    return board[row * BOARDSIZE + col];
}

std::string Go::sgfToStd(const std::string& sgfMove){
    int c = int(sgfMove[2]);
    int r = int(sgfMove[3]);
    return std::string{sgfMove[0], ' ', (c < 105 ? (char)std::toupper(c) : (char)std::toupper(c+1))} + std::to_string(116-r);
}

bool Go::saveSgf() const{
    return saveSgf(".");
}

bool Go::saveSgf(const std::string& sgfDir) const {
    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::ostringstream filename;
    filename << sgfDir << '/';
    filename << player1 << "_vs_" << player2 << '_';
    filename << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S");
    std::ofstream file{filename.str() + ".sgf"};
    if(not file)
        return false;
    file << metaData << "GN[" << filename.str().substr(sgfDir.size()+1,std::string::npos) << "]\nDT[" << std::put_time(std::localtime(&t), "%Y-%m-%d") << "]\n";
    for(std::string move : moves){
        file << ';' << move << '\n';
    }
    file << ')';
    file.close();
    return true;
}

bool Go::loadSgf(const std::string& filename){
    if(moves.size() != 0)
        return false;
    std::ifstream file{filename};
    if(not file)
        return false;
    std::string n;
    
    /*while(file >> n){
        if
    }*/
    return true;
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
    int index = row * BOARDSIZE + col;
    Stone& cs = board[index];
    if(not moveInRange(index))
        return false;
    if(cs != Stone::Empty)
        return false;
    
    cs = s; //place the move temporarily
    bool doesCapture = false;
    std::vector<int> shp{};
    for(int i = 0; i < 4; i++){
        int newIndex = index + (2 * (i%2) - 1) * ((BOARDSIZE-1) * (i/2) + 1); //index + (-1, +1, -BOARDSIZE, +BOARDSIZE)
        if(moveInRange(newIndex) and int(board[newIndex]) == 3-int(s)){
            if(shape(newIndex, shp) == 0){
                doesCapture = true;
                for(int cptIndex : shp){
                    board[cptIndex] = Stone::Empty;
                    captures[int(s)]++;
                }
            }
            shp.clear();
        }
    }

    if(not doesCapture and shape(index, shp) == 0)
        return bool(cs = Stone::Empty); //returns false
    //End of tests

    moves.push_back(std::string{(s==Stone::Black ? 'B' : 'W'), '[', char(col + 97), char(row + 97),']'}); //Convert move(row, col, s) -> sgf move
    return true;
}

inline bool Go::moveInRange(const int index) const {
    return (index >= 0) and (index < BOARDSIZE * BOARDSIZE);
}

//Given the index of a stone, this function determines which shape the stone belongs to and calculates the indices of all stones that are part of that shape, including the original stone. Also returns the liberty count of the shape.
int Go::shape(const int index, std::vector<int>& res) const {
    std::vector<int> libs;
    shape(index, res, libs);
    return libs.size();
}

void Go::shape(const int index, std::vector<int>& res, std::vector<int>& liberties) const {
    res.push_back(index);
    for(int i = 0; i < 4; i++){
        int newIndex = index + (2 * (i%2) - 1) * ((BOARDSIZE-1) * (i/2) + 1); //index + (-1, +1, -BOARDSIZE, +BOARDSIZE)
        if(moveInRange(newIndex)){
            if (board[newIndex] == board[index] and std::find(res.begin(), res.end(), newIndex) == res.end())
                shape(newIndex, res, liberties);
            else if (board[newIndex] == Stone::Empty and std::find(liberties.begin(), liberties.end(), newIndex) == liberties.end())
                liberties.push_back(newIndex);
        }       
    }
}

void Go::setHandiMeta(const int n) {
    switch (n){
        case 2:
            metaData += "HA[" + std::to_string(n) + "]\nAB[pd][dp]";
            break;
        case 3:
            setHandiMeta(2);
            metaData += "[pp]";
            break;
        case 4:
            setHandiMeta(3);
            metaData += "[dd]";
            break;
        case 5:
            setHandiMeta(4);
            metaData += "[jj]";
            break;
        case 6:
            setHandiMeta(4);
            metaData += "[dj][pj]";
            break;
        case 7:
            setHandiMeta(6);
            metaData += "[jj]";
            break;
        case 8:
            setHandiMeta(6);
            metaData += "[jd][jp]";
            break;
        case 9:
            setHandiMeta(8);
            metaData += "[jj]";
            break;
        default:
            break;
    }
}


bool Go::setHandi(const int n){
    if(moves.size() != 0)
        return false;
    switch (n){
        case 1:
            captures[0] += (KOMI + 0.5); //equivalent to komi = -0.5
            return true;
        case 2:
            board[3 * BOARDSIZE + 15] = Stone::Black;
            board[15 * BOARDSIZE + 3] = Stone::Black;
            return setHandi(1);
        case 3:
            board[15 * BOARDSIZE + 15] = Stone::Black;
            return setHandi(2);
        case 4:
            board[3 * BOARDSIZE + 3] = Stone::Black;
            return setHandi(3);
        case 5:
            board[9 * BOARDSIZE + 9] = Stone::Black;
            return setHandi(4);
        case 6:
            board[9 * BOARDSIZE + 3] = Stone::Black;
            board[9 * BOARDSIZE + 15] = Stone::Black;
            return setHandi(4);
        case 7:
            board[9 * BOARDSIZE + 9] = Stone::Black;
            return setHandi(6);
        case 8:
            board[3 * BOARDSIZE + 9] = Stone::Black;
            board[15 * BOARDSIZE + 9] = Stone::Black;
            return setHandi(6);
        case 9:
            board[9 * BOARDSIZE + 9] = Stone::Black;
            return setHandi(8);
        default:
            return false;
    }
}