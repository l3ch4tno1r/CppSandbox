#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Yes ! It is a bit overkill xD

template<size_t L, size_t C>
struct Board
{
    Board(const char* grid)
    {
        char* p = begin();
        char* e = end();

        while (*grid != '\0' && p != e)
            *p++ = *grid++;
    }

    constexpr size_t Lines()   const { return L; }
    constexpr size_t Columns() const { return C; }

    char Grid[L][C];

    char  operator()(size_t i, size_t j) const { return this->Grid[i][j]; }
    char& operator()(size_t i, size_t j) { return this->Grid[i][j]; }

    const char* begin() const { return (char*)Grid; }
    char* begin() { return (char*)Grid; }

    const char* end() const { return (char*)Grid + C * L; }
    char* end() { return (char*)Grid + C * L; }

    bool Play(char chip, size_t pos)
    {
        int i = L;

        while (this->Grid[i][pos] != '.' && i >= 0)
            --i;

        if (i < 0)
            return false;

        this->Grid[i][pos] = chip;

        return true;
    }
};

using Board7L9C = Board<7, 9>;

//////////////////////
//-- Stream utils --//
//////////////////////

template<size_t L, size_t C>
ostream& operator<<(ostream& stream, const Board<L, C>& board)
{
    for (int i = 0; i < L; ++i)
    {
        for (int j = 0; j < C; ++j)
            stream << board(i, j);

        stream << endl;
    }

    return stream;
}

template<size_t L, size_t C>
istream& operator>>(istream& stream, Board<L, C>& board)
{
    for (char& c : board)
        cin >> c;

    return stream;
}

/////////////////
//-- MiniMax --//
/////////////////

struct Max;

struct Min
{
    using OppositeType = Max;

    int operator()(int a, int b) const { return std::min(a, b); }

    static constexpr int Boundary() { return std::numeric_limits<int>::max(); }
    
    inline static const std::string Name = "Min";
};

struct Max
{
    using OppositeType = Min;

    int operator()(int a, int b) const { return std::max(a, b); }

    static constexpr int Boundary() { return std::numeric_limits<int>::min(); }

    inline static const std::string Name = "Max";
};

template<typename MiniMaxType, typename BoardType, typename EvalFunc>
int MiniMax(
    size_t    depth,
    BoardType board,
    char      myChip,
    char      oppChip,
    bool      me,
    size_t    action,
    EvalFunc  F)
{
    using OppositeType = typename MiniMaxType::OppositeType;

    if (!board.Play(me ? myChip : oppChip, action))
        return 0;

    if (depth == 0)
        return F(board, myChip, oppChip);

    int value = MiniMaxType::Boundary();

    for (size_t i = 0; i < board.Columns(); ++i)
        value = MiniMaxType()(value, MiniMax<OppositeType>(depth - 1, board, myChip, oppChip, !me, i, F));

    return value;
}

////////////////////
//-- Evaluation --//
////////////////////

template<typename BoardType>
int EvaluateBoard1(const BoardType& board, char myChip, char oppChip)
{
    int score = 0;

    for (size_t i = 0; i < board.Columns(); ++i)
    {
        char chip = '.';

        for (size_t j = 0; j < board.Lines(); ++j)
        {
            chip = board(j, i);

            if (chip != '.')
                break;
        }

        score += 1 * (chip == myChip) + (-1) * (chip == oppChip);
    }

    return score;
}

template<typename BoardType>
int EvaluateBoard2(const BoardType& board, char myChip, char oppChip)
{
    return 0;
}

int main()
{
    Board7L9C board(
        "........."
        "........."
        "........."
        "....0...."
        "....1...."
        "...01.00."
        "..111001.");

    char myChip = '0', oppChip = '1';

    std::cout << EvaluateBoard1(board, myChip, oppChip) << std::endl;
    
    int bestScore = std::numeric_limits<int>::min();
    int bestMove  = -1;

    for (int i = 0; i < board.Columns(); ++i)
    {
        int temp = MiniMax<Min>(0, board, myChip, oppChip, false, i, EvaluateBoard2<Board7L9C>);

        if (temp < bestScore)
            continue;

        bestScore = temp;
        bestMove  = i;
    }

    std::cout << bestMove << std::endl;

    board.Play(myChip, bestMove);

    std::cout << board;

	std::cin.get();
}