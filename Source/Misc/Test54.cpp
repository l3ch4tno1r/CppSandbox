#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    std::stringstream sstr{ "1 2 4 3 1 6 1 0" };

    std::istream_iterator<int> begin{ sstr }, end{};

    std::vector<int> vec;

    std::transform(begin, end, std::back_inserter(vec), [](int x)
    {
        return 2 * x;
    });

    for (int x : vec)
        std::cout << x << std::endl;

    std::cin.get();
}