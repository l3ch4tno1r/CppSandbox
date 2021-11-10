#include <iostream>
#include <string>

template<class Entity>
class Table;

template<class DerivedType>
class Query
{
public:
	std::string ToString() const { return this->Derived().ToString(); }

private:
	const DerivedType& Derived() const { return static_cast<const DerivedType&>(*this); }
	      DerivedType& Derived()       { return static_cast<DerivedType&>(*this); }
};

class Contact
{};

template<>
class Table<Contact> : public Query<Table<Contact>>
{};

class DirectoryDB
{
public:
	using TContact = Table<Contact>;

	const TContact& Contacts() const { return m_Contact; }

private:
	TContact m_Contact{};
};

int main()
{
	std::cin.get();
}