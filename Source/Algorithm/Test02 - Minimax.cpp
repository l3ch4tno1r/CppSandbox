#include <iostream>

struct Max;

struct Min
{
    using OppositeType = Max;
    using CompareType  = std::less<int>;

    int operator()(int a, int b) const { return std::min(a, b); }

    static int Boundary() { return std::numeric_limits<int>::max(); }
};

struct Max
{
    using OppositeType = Min;
    using CompareType  = std::greater<int>;

    int operator()(int a, int b) const { return std::max(a, b); }

    static int Boundary() { return std::numeric_limits<int>::min(); }
};

template<typename MiniMaxType>
void MiniMax(size_t depth)
{
    using OppositeType = typename MiniMaxType::OppositeType;
    using CompareType  = typename MiniMaxType::CompareType;

    if (depth == 0)
        return;

    std::cout << MiniMaxType()(2, 3) << std::endl;

    MiniMax<OppositeType>(depth - 1);
}

int main()
{
    MiniMax<Max>(4);

	std::cin.get();
}