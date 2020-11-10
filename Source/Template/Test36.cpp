#include <iostream>

template<size_t N>
class Vector
{
public:
	enum
	{
		X_Accessible = N >= 1,
		Y_Accessible = N >= 2,
		Z_Accessible = N >= 3
	};

private:
	float m_Data[N];
};

int main()
{
	std::cin.get();
}