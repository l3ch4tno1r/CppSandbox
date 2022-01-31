#include <iostream>

#include "Utilities/Experimental/LiteralStringGenerator.h"
#include "Utilities/Source/NameOfType.h"

template<typename _EntityType>
struct TableName
{
	static constexpr std::string_view GetName()
	{
		const std::string_view& view{ LCN::NameOf<_EntityType> };

		const size_t pos  = view.find(' ') + 1;
		const size_t size = view.size() - pos;

		return { view.data() + pos, size };
	}

	inline static constexpr std::string_view Name = GetName();

	static constexpr size_t Size() { return Name.size(); }

	static constexpr char Value(size_t Idx) { return Name[Idx]; }
};

struct Contact
{};

int main()
{
	using ContactTabName = TableName<Contact>;
	using ContactVarName = LCN::ToLower<ContactTabName>;
	using ContactVARName = LCN::ToUpper<ContactTabName>;

	std::cout << LCN::Literal<ContactTabName> << '\n';
	std::cout << LCN::Literal<ContactVarName> << '\n';
	std::cout << LCN::Literal<ContactVARName> << '\n';

	constexpr size_t size = LCN::ToString<size_t, 505>::Size();

	std::cout << LCN::Literal<LCN::ToString<size_t,  505>>  << '\n';
	std::cout << LCN::Literal<LCN::ToString<int,    -1505>> << '\n';
	std::cout << LCN::Literal<LCN::ToString<int,    -2>>    << '\n';
	std::cout << LCN::Literal<LCN::ToString<int,    -0>>    << '\n';
	std::cout << LCN::Literal<LCN::ToString<int,    -153>>  << '\n';
	std::cout << LCN::Literal<LCN::ToString<size_t,  26>>   << '\n';

	using Contact0VarName = LCN::ConcatLiterals<ContactVarName, LCN::ToString<size_t, 0>>;
	using Contact1VarName = LCN::ConcatLiterals<ContactVarName, LCN::ToString<size_t, 1>>;
	using Contact2VarName = LCN::ConcatLiterals<ContactVarName, LCN::ToString<size_t, 2>>;

	std::cout << LCN::Literal<Contact0VarName> << '\n';
	std::cout << LCN::Literal<Contact1VarName> << '\n';
	std::cout << LCN::Literal<Contact2VarName> << '\n';

	std::cin.get();
}