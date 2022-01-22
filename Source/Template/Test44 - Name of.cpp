#include <iostream>
#include <string>
#include <string_view>

template<typename _Type>
struct NameOfType
{
	static constexpr std::string_view Value()
	{
		std::string_view name{ __FUNCSIG__ };

		size_t begin = name.find("NameOfType") + 11;
		size_t end   = name.find(">::", begin);

		return { name.data() + begin, end - begin };
	}
};

template<typename _Type>
constexpr std::string_view NameOf = NameOfType<_Type>::Value();

void PrintString(std::string_view view)
{
	std::cout << view << '\n';
}

int main()
{
	const auto& ref = std::cout;

	std::cout << NameOf<int>           << '\n';
	std::cout << NameOf<int&>          << '\n';
	std::cout << NameOf<decltype(ref)> << '\n';

	PrintString("Hello world");

	std::string str{ "Bonjour le monde" };

	PrintString(str);

	std::cin.get();
}