#include <iostream>
#include <array>

//////////////////////////

template<size_t Size, size_t Idx = 0, char ... c>
struct ConstStringHelper : ConstStringHelper<Size, Idx + 1, c..., '/'>
{};

template<size_t Size, char ... c>
struct ConstStringHelper<Size, Size, c...>
{
	static constexpr char Str[Size] = { c... };
};

template<size_t N>
constexpr size_t SizeofString(const char(&)[N])
{
	return N - 1;
}

////////////////
//-- Border --//
////////////////

#define HASCPP17 0

#if HASCPP17
#define BORDER Border
#else
#define BORDER FormatBorder
#endif

template<size_t N>
class BORDER
{
public:
	BORDER(const char(&str)[N]) :
		m_Str{ str }
	{}

private:
	const char(&m_Str)[N];

	template<size_t _N>
	friend void operator<<(std::ostream&, const BORDER<_N>&);
};

#if !HASCPP17
template<size_t N>
FormatBorder<N> Border(const char(&str)[N])
{
	return { str };
}
#endif

template<size_t N>
void operator<<(std::ostream& os, const BORDER<N>& borderStr)
{
	const char(&border)[] = ConstStringHelper<N + 2 * SizeofString("//--") + 1>::Str;

	os << border << '\n';
	os << "//-- " << borderStr.m_Str << " --//" << '\n';
	os << border << '\n';
}

int main()
{
	std::cout << '\n' << Border("Hello gentlemen !");
	std::cout << '\n' << Border("The name is Bond, James Bond !");
	std::cout << '\n' << Border("Hello gentlemen !");
	std::cout << '\n' << Border("");

	std::cin.get();
}