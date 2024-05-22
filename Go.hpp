#ifndef GO_HPP
#define GO_HPP
#include <array>
#include <string>

const int BOARDSIZE = 19;

enum class Player {Empty, Black, White};

class Go {
    public:
        Go();
        Go(std::string sgf);
        Player player(const int row, const int col) const;
        static std::string sgfToStd(const std::string& sgfMove);
        //static std::string stdToSgf(const std::string& stdMove);
    private:
        std::array<Player, BOARDSIZE*BOARDSIZE> board;
};

#endif