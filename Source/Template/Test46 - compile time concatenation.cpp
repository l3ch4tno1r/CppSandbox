#include <iostream>

#include "Utilities/Source/NameOfType.h"

#include "Utilities/Experimental/TableNaming.h"
#include "Utilities/Experimental/ColumnNaming.h"

using ConstCharRef = const char(&)[];

template<typename Generator, size_t Size, size_t Idx = 0, char ... c>
struct ConstStringHelper : ConstStringHelper<Generator, Size, Idx + 1, c..., Generator::Value(Idx)>
{
	using BaseType = ConstStringHelper<Generator, Size, Idx + 1, c..., Generator::Value(Idx)>;
};

template<typename Generator, size_t Size, char ... c>
struct ConstStringHelper<Generator, Size, Size, c...>
{
	static constexpr char Str[Size] = { c... };
};

struct GeneratorTest
{
	static constexpr char Value(size_t Idx)
	{
		return 'A' + Idx;
	}
};

template<typename Type1, typename Type2>
struct ConcatName
{
	static constexpr size_t Size() { return LCN::NameOf<Type1>.size() + LCN::NameOf<Type2>.size(); }

	static constexpr char Value(size_t Idx)
	{
		const size_t size1 = LCN::NameOf<Type1>.size();

		if (Idx < size1)
			return LCN::NameOf<Type1>[Idx];

		return LCN::NameOf<Type2>[Idx - size1];
	}

	inline static std::string_view Str = ConstStringHelper<ConcatName, Size()>::Str;
};

template<size_t I>
struct ToString
{
	static constexpr size_t Size()
	{
		if (I == 0)
			return 1;

		size_t result = 0;

		for (size_t i = I; i != 0; i /= 10)
			++result;

		return result;
	}

	static constexpr char Value(const size_t Idx)
	{
		size_t idx = Size() - Idx - 1;
		size_t div = 1;

		for (size_t i = 0; i < idx; ++i)
			div *= 10;

		return (I / div) % 10 + '0';
	}
};

//////////////
//-- Test --//
//////////////

template<typename _TableType>
struct TableNameGenerator;

template<class _EntityType, size_t I>
struct TableBase
{};

template<class _EntityType, size_t I = 0>
struct Table;

template<typename _EntityType, size_t I>
struct TableNameGenerator<Table<_EntityType, I>>
{
	static constexpr size_t Size()
	{
		const std::string_view& name{ LCN::NameOf<_EntityType> };

		size_t idx   = name.find(' ') + 1;
		size_t count = name.size() - idx;

		return count + ToString<I>::Size();
	}

	static constexpr char Value(size_t Idx)
	{		
		const std::string_view& name{ LCN::NameOf<_EntityType> };

		size_t idx   = name.find(' ') + 1;
		size_t count = name.size() - idx;

		if(Idx >= count)
			return ToString<I>::Value(Idx - count);

		const char c = name[idx + Idx];

		return ('A' <= c && c <= 'Z') ? c - 'A' + 'a' : c;
	}
};

template<class _EntityType, size_t I = 0>
constexpr Table<_EntityType, I> table;

struct Contact
{};

template<size_t I>
struct Table<Contact, I> : TableBase<Contact, I>
{};

int main()
{
	using ContactNameGen = TableNameGenerator<Table<Contact, 20>>;

	std::cout << ConstStringHelper<ContactNameGen, ContactNameGen::Size()>::Str << '\n';

	std::cin.get();
}