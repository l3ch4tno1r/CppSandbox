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

	std::cout << LCN::Literal<ContactTabName> << '\n';
	std::cout << LCN::Literal<ContactVarName> << '\n';

	std::cin.get();
}