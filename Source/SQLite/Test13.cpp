#include <iostream>
#include <string>
#include <string_view>

#include "SQLiteEntity/Expression.h"

template<class Derived>
std::ostream& operator<<(std::ostream& os, const LCN::SQLiteEntity::Expression<Derived>& expr)
{
	for (size_t i = 0; i < expr.Size(); ++i)
		os << expr[i];

	return os;
}

template<class Owner>
class DBTable;

template<class Owner, typename PropType>
class DBProperty : public LCN::SQLiteEntity::Variable<DBProperty<Owner, PropType>>
{
public:
	using OwnerType = Owner;
	using ValType   = PropType;

	inline size_t Size() const
	{
		return Owner::TableName.size() + m_Name.size() + 1;
	}

	inline char operator[](size_t i) const
	{
		if (i < Owner::TableName.size())
			return Owner::TableName[i];

		if (i == Owner::TableName.size())
			return '.';

		i -= Owner::TableName.size() + 1;

		return m_Name[i];
	}

private:
	const Owner& m_Owner;
	std::string_view m_Name;

	DBProperty(const Owner& owner, std::string_view name) :
		m_Owner(owner),
		m_Name(name)
	{}

	friend Owner;
};

class Contact
{
public:

private:
	uint32_t m_Id;
	std::string m_FirstName;
	std::string m_LastName;
};

template<>
class DBTable<Contact>
{
public:
	template<typename PropType>
	using ContactProp = DBProperty<DBTable<Contact>, PropType>;

	inline static const std::string TableName{ "Contact" };

	ContactProp<uint32_t> Id() const
	{
		static std::string_view id{ "Id" };
		return { *this, id };
	}

	ContactProp<std::string> FirstName() const
	{
		static std::string_view firstName{ "FirstName" };
		return { *this, firstName };
	}
};

template<typename T>
class Constant : public LCN::SQLiteEntity::Variable<Constant<T>>
{
public:
	template<typename String>
	Constant(String&& str) :
		m_Value(std::forward<String>(str))
	{}

private:
	std::string m_Value;
};

int main()
{
	DBTable<Contact> contact;

	std::cout << contact.Id()        << std::endl;
	std::cout << contact.FirstName() << std::endl;
	std::cout << (contact.FirstName() == contact.Id() && contact.FirstName() != contact.Id()) << std::endl;

	

	std::cin.get();
}