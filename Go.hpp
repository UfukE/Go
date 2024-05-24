#ifndef GO_HPP
#define GO_HPP
#include <array>
#include <vector>
#include <string>

const int BOARDSIZE = 19;
const float KOMI = 7.5;

enum class Stone {Empty, Black, White};

class Go {
    public:
        Go();
        Go(const std::string& name1, const std::string& name2);
        Go(const std::string& sgf);
        Stone stone(const int row, const int col) const;
        void saveSgf(const std::string& sgfDir) const;

        static std::string sgfToStd(const std::string& sgfMove);
    private:
        std::array<Stone, BOARDSIZE*BOARDSIZE> board;
        std::array<int, 2> captures;
        std::vector<std::string> moves;
        std::string player1;
        std::string player2;
        bool gameOver;

};

#endif