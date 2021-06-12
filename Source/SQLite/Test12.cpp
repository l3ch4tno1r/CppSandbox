#include <iostream>
#include <tuple>
#include <string>
#include <memory>

//-- Query --//

template<class Entity>
class Table;

template<class Entity>
class Query
{
public:
	virtual std::string ToString() const = 0;
};

template<class Entity>
class SelectQuery : public Query<Entity>
{};

template<class Entity>
class WhereQuery : public Query<Entity>
{};

template<class Entity>
class FromQuery : public Query<Entity>
{
public:
	//std::unique_ptr<
};

//-- Database --//

template<class Entity>
class Table
{
public:
	Query<Entity> From() const
};

template<class ... Entities>
class Database
{
public:
	template<class Entity>
	Table<Entity>& Tables() { return std::get<Table<Entity>>(m_Tables); }

private:
	std::tuple<Table<Entities>...> m_Tables;
};

//-- Test classes --//

class Contact
{};

class PhoneNumbers
{};

//-- Usings --//
using AppDatabase = Database<Contact, PhoneNumbers>;

int main()
{
	AppDatabase db;

	auto& contacts     = db.Tables<Contact>();
	auto& phonenumbers = db.Tables<PhoneNumbers>();

	std::cin.get();
}