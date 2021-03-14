#include <iostream>
#include <string>
#include <sstream>

template<class Target>
class Entity
{
public:
	Entity() : m_EntityId(++tl_EntityCount) {}
	~Entity() { --tl_EntityCount; }

	unsigned int EntityId() const { return m_EntityId; }

private:
	thread_local static unsigned int tl_EntityCount;

	unsigned int m_EntityId;
};

template<class Target>
thread_local unsigned int Entity<Target>::tl_EntityCount = 0;

template<class Target>
class Table;

class Contact
{
public:
	unsigned int& Id()       { return m_Id; }
	unsigned int  Id() const { return m_Id; }

	std::string&       LastName()       { return m_LastName; }
	const std::string& LastName() const { return m_LastName; }

	std::string&       FirstName()       { return m_FirstName; }
	const std::string& FirstName() const { return m_FirstName; }

	std::string&       Birthday()       { return m_Birthday; }
	const std::string& Birthday() const { return m_Birthday; }

private:
	unsigned int m_Id;

	std::string m_LastName;
	std::string m_FirstName;
	std::string m_Birthday;
};

template<>
class Table<Contact> : public Entity<Contact>
{
public:
	Table()
	{
		std::stringstream sstr;
		
		sstr << c_TableName << this->EntityId();

		m_TableName = sstr.str();

		m_LastNameColum  = m_TableName + '.' + c_LastName;
		m_FirstNameColum = m_TableName + '.' + c_FirstName;
		m_BirthdayColum  = m_TableName + '.' + c_Birthday;
	}

	const std::string& LastName()  const { return m_LastNameColum; }
	const std::string& FirstName() const { return m_FirstNameColum; }
	const std::string& Birthday()  const { return m_BirthdayColum; }

	const std::string& TableName() const { return m_TableName; }

private:
	const char* c_TableName = "Contact";

	const char* c_LastName  = "LastName";
	const char* c_FirstName = "FirstName";
	const char* c_Birthday  = "Birthday";

	std::string m_TableName;

	std::string m_LastNameColum;
	std::string m_FirstNameColum;
	std::string m_BirthdayColum;
};

int main()
{
	Table<Contact> contact1;

	std::cout << contact1.LastName() << std::endl;

	std::cin.get();
}