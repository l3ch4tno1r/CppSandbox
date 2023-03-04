#include <iostream>
#include <sstream>

namespace LCN::Parsing
{
#pragma region Check single char
	////////////////
	//-- CheckC --//
	////////////////

	struct CheckC
	{
		char c;
	};

	std::istream&
	operator>>(
		std::istream& is,
		const CheckC check)
	{
		char c;
		is >> c;

		if (check.c != c)
			is.setstate(std::ios::failbit);

		return is;
	}

	CheckC
	Check(
		char c)
	{
		return { c };
	}

	struct CheckNC
	{
		size_t n;
		CheckC c;
	};

	inline
	CheckNC
	operator*(
		const size_t n,
		const CheckC c)
	{
		return { n, c };
	}

	inline
	CheckNC
	operator*(
		const CheckC c,
		const size_t n)
	{
		return n * c;
	}

	std::istream&
	operator>>(
		std::istream& is,
		const CheckNC check)
	{
		for (size_t i = 0; i < check.n; ++i)
			is >> check.c;

		return is;
	}

#pragma endregion

#pragma region Check string
	////////////////
	//-- CheckS --//
	////////////////

	struct CheckS
	{
		std::string_view sv;
	};

	std::istream&
	operator>>(
		std::istream& is,
		const CheckS check)
	{
		for (char c : check.sv)
			is >> Check(c);

		return is;
	}

	template<typename _Str>
	CheckS
	Check(
		_Str&& str)
	{
		return { std::forward<_Str>(str) };
	}

#pragma endregion
}

int main()
{
	using LCN::Parsing::Check;

	try
	{
		std::stringstream sstr{ "bbbb [1;2;3;4;5;6] eeee" };
		sstr.exceptions(std::ios::failbit | std::ios::badbit);

		int tab[6];
		auto it = std::begin(tab), end = std::end(tab);

		sstr >> Check('b') * 4 >> Check('[') >> *(it++);

		for (; it != end; ++it)
			sstr >> Check(';') >> *it;

		sstr >> Check(']') >> 4 * Check('e');
	}
	catch (const std::ios::failure& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}