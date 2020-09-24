#include <iostream>

struct Vec
{
	float x, y, z;

	float& operator[](size_t idx)
	{
		return *(((float*)this) + idx);
	}

	float operator[](size_t idx) const
	{
		return *(((float*)this) + idx);
	}
};

std::ostream& operator<<(std::ostream& stream, const Vec& vec)
{
	stream << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';

	return stream;
}

int main()
{
	Vec vec;
	float* pvec = (float*)&vec;

	for (float* ptr = pvec; ptr < pvec + 3; ++ptr)
		*ptr = 1.0f;

	std::cout << vec << std::endl;

	vec[0] = 2.0f;
	vec[1] = 3.0f;
	vec[2] = 4.0f;

	std::cout << vec << std::endl;

	std::cin.get();
}