#include <iostream>

struct Fraction
{
	unsigned Num;
	unsigned Den;
};

constexpr
std::weak_ordering
operator<=>(
	const Fraction& a,
	const Fraction& b)
{
	return (a.Num * b.Den) <=> (b.Num * a.Den);
}

constexpr
bool
operator==(
	const Fraction& a,
	const Fraction& b)
{
	return std::is_eq(a <=> b);
}

int main()
{
	constexpr Fraction a{ 1, 3 }, b{ 2, 6 }, c{ 1, 2 }, d{ 1, 4 };

	static_assert(a == b);
	static_assert(a <= c);
	static_assert(a >= d);
	static_assert(a != d);

	std::cin.get();
}
