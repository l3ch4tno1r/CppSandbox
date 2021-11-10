#include <iostream>

class Hysteresis
{
public:
    Hysteresis(int low, int high, bool state = false) :
        m_Lo(low),
        m_Hi(high),
        m_State(state)
    {}

    bool operator()(int val)
    {
        if (val < m_Lo)
            m_State = false;

        if (val > m_Hi)
            m_State = true;

        return m_State;
    }

private:
    int m_Hi, m_Lo;
    bool m_State;
};

int main()
{
    int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2 };

    Hysteresis comp{ 0, 4 };

    for (int i : values)
        std::cout << "comp(" << i << ") = " << comp(i) << std::endl;

	std::cin.get();
}