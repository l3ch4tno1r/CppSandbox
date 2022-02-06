#include <iostream>

#include "Utilities/Experimental/LiteralStringGenerator.h"
#include "Utilities/Source/NameOfType.h"

template<typename _EntityType>
struct TableName
{
	static constexpr std::string_view GetName()
	{
		const std::string_view& view{ LCN::NameOf<_EntityType> };

		const size_t pos = view.find(' ') + 1;
		const size_t size = view.size() - pos;

		return { view.data() + pos, size };
	}

	inline static constexpr std::string_view Name = GetName();

	static constexpr size_t Size() { return Name.size(); }

	static constexpr char Value(size_t Idx) { return Name[Idx]; }
};

template<typename _EntityType, size_t _I>
struct VarName : LCN::ConcatLiterals<LCN::ToLower<TableName<_EntityType>>, LCN::ToString<size_t, _I>>
{};

template<typename _EntityType, size_t _I>
struct TableBase
{
	using TableName = TableName<_EntityType>;
	using VarName   = VarName<_EntityType, _I>;
};

template<typename _EntityType, size_t _I>
struct Table;

struct Contact
{};

template<size_t _I>
struct Table<Contact, _I> : TableBase<Contact, _I>
{};

int main()
{
	using TContact = Table<Contact, 0>;

	std::cout << LCN::Literal<TContact::TableName> << '\n';
	std::cout << LCN::Literal<TContact::VarName> << '\n';

	std::cin.get();
}