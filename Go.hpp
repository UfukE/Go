#ifndef GO_HPP
#define GO_HPP
#include <array>
#include <vector>
#include <string>

constexpr int BOARDSIZE = 19;
constexpr float KOMI = 7.5;
constexpr int HANDICAP = 0;

enum class Stone {Empty, Black, White};

class Go {
    public:
        Go();
        Go(const std::string& name1, const std::string& name2);
        Go(const std::string& sgf);
        inline Stone stone(const int row, const int col) const;

        bool saveSgf() const;
        bool saveSgf(const std::string& sgfDir) const;
        bool loadSgf(const std::string& filename);
        std::string toString() const;
        std::string toString(const char empty, const char black, const char white) const;
        bool play(std::string stdMove);
        bool makeMove(const int row, const int col, const Stone s);
        void setHandi(const int n);
        static std::string sgfToStd(const std::string& sgfMove);
    private:
        std::array<Stone, BOARDSIZE*BOARDSIZE> board;
        std::array<int, 2> captures;
        std::vector<std::string> moves;
        std::string player1;
        std::string player2;
        std::string metaData;
        bool gameOver;
        inline bool moveInRange(const int index) const;
        int shape(const int index, std::vector<int>& res) const;
        void shape(const int index, std::vector<int>& res, std::vector<int>& liberties) const;
};

#endif