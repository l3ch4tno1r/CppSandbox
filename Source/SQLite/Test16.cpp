#include <iostream>
#include <string>
#include <algorithm>

#include "Utilities/Source/NameOfType.h"

////////////////////////
//-- SQL statements --//
////////////////////////

template<class _Derived>
class SQLStatementBase
{
public:
	inline std::string ToString() const { return this->Derived().ToString(); }

	inline operator std::string() const { return this->ToString(); }
protected:
	const _Derived Derived() const { return static_cast<const _Derived&>(*this); }
	      _Derived Derived()       { return static_cast<      _Derived&>(*this); }
};

template<class _TableType>
class FromStatement : public SQLStatementBase<FromStatement<_TableType>>
{
public:
	using BaseType = SQLStatementBase<FromStatement<_TableType>>;

	inline std::string ToString() const
	{
		static const std::string result{ "select * from " };

		return result + _TableType::TableName + ' ' + _TableType::VarName;
	}

	template<class _TableType>
	friend FromStatement<_TableType> From(const _TableType&);

private:
	FromStatement() = default;
};

template<class _TableType>
FromStatement<_TableType> From(const _TableType&)
{
	return {};
}

/////////////////////
//-- Concrete DB --//
/////////////////////

template<class _EntityType, size_t I = 0>
struct TableBase
{
	static std::string GetTableName()
	{
		const std::string_view& name{ LCN::NameOf<_EntityType> };

		size_t idx   = name.find(' ') + 1;
		size_t count = name.size() - idx;

		std::string result{ name.data() + idx, count };

		return result;
	}

	static std::string GetVarName()
	{		
		std::string result = GetTableName();

		std::transform(
			result.begin(),
			result.end(),
			result.begin(),
			[](char c)
			{
				return std::tolower(c);
			});

		return result + std::to_string(I);
	}

	inline static const std::string TableName = GetTableName();
	inline static const std::string VarName   = GetVarName();
};

template<class _EntityType, size_t I = 0>
struct Table;

template<class _EntityType, size_t I = 0>
constexpr Table<_EntityType, I> table;

struct Contact
{};

template<size_t I>
struct Table<Contact, I> : TableBase<Contact, I>
{};

int main()
{
	auto statement = From(table<Contact>);

	std::cout << (std::string)statement << '\n';

	std::cin.get();
}