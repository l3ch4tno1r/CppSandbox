#include <iostream>
#include <functional>
#include <vector>
#include <string>

#include <Utilities/External/sqlite3/sqlite3.h>
#include <Utilities/Source/ErrorHandling.h>

struct Contact
{
	uint32_t Id;
	std::string FirstName, LastName;

	template<typename Str1, typename Str2>
	Contact(uint32_t id, Str1&& firstname, Str2&& lastname) :
		Id(id),
		FirstName(std::forward<Str1>(firstname)),
		LastName(std::forward<Str2>(lastname))
	{}

	Contact(const Contact& other) :
		Id(other.Id),
		FirstName(other.FirstName),
		LastName(other.LastName)
	{}

	Contact(Contact&& other) noexcept :
		Id(other.Id),
		FirstName(std::move(other.FirstName)),
		LastName(std::move(other.LastName))
	{}
};

std::ostream& operator<<(std::ostream& os, const Contact& contact)
{
	os << contact.Id << " - " << contact.FirstName << ' ' << contact.LastName;

	return os;
}

/*
int callback(void* a_param, int argc, char** argv, char** column)
{
	ASSERT(argc == 3);

	std::vector<Contact>& contactList = *(std::vector<Contact>*)(a_param);

	contactList.emplace_back(argv[0], argv[1]);

	return 0;
}
*/

class Statement
{
public:
	class Iterator
	{
	public:
		Iterator(Statement& statement, int status = SQLITE_DONE) :
			m_Target(statement),
			m_Status(status)
		{
			m_Target.Reset();
		}

		Iterator& operator++()
		{
			m_Status = m_Target.NextStep();

			return *this;
		}

		Statement& operator*()  { return  m_Target; }
		Statement* operator->() { return &m_Target; }

	private:
		Statement& m_Target;
		int m_Status{ SQLITE_ROW };

		friend bool operator!=(const Iterator& it1, const Iterator& it2);
	};

	Iterator begin() { return { *this, SQLITE_ROW }; }
	Iterator end()   { return { *this, SQLITE_DONE }; }

	~Statement()
	{
		sqlite3_finalize(m_Statement);
	}

private:
	Statement() = default;

	Statement(const Statement& other) :
		m_Statement(other.m_Statement),
		m_Status(other.m_Status)
	{}

	int Reset()
	{
		return sqlite3_reset(m_Statement);
	}

public:
	int NextStep()
	{
		return sqlite3_step(m_Statement);
	}

private:
	sqlite3_stmt* m_Statement{ nullptr };
	int m_Status{ SQLITE_DONE };

	friend Statement CreateStatement(sqlite3* db, const char* query);
};

bool operator!=(const Statement::Iterator& it1, const Statement::Iterator& it2)
{
	return it1.m_Status != it2.m_Status;
}

Statement CreateStatement(sqlite3* db, const char* query)
{
	Statement result;

	result.m_Status = sqlite3_prepare_v2(
		db,
		query,
		-1,
		&result.m_Statement,
		nullptr);

	if (result.m_Status != SQLITE_OK)
		throw std::exception(sqlite3_errmsg(db));

	return result;
}

class Stmt
{
public:
	Stmt(sqlite3_stmt* stmt) :
		m_Stmt(stmt)
	{}

	Stmt(const Stmt& other) :
		m_Stmt(other.m_Stmt)
	{}

	template<typename T>
	T GetColumnValue(uint32_t) const;

	template<>
	int GetColumnValue(uint32_t i) const { return sqlite3_column_int(m_Stmt, i); }

	template<>
	const char* GetColumnValue(uint32_t i) const { return (const char*)sqlite3_column_text(m_Stmt, i); }

private:
	sqlite3_stmt* m_Stmt{ nullptr };

	friend class StmtIterator;
};

class StmtIterator
{
public:
	StmtIterator() = default;

	template<typename StmtType>
	StmtIterator(StmtType&& stmt) :
		m_Stmt(std::forward<StmtType>(stmt)),
		m_Status(SQLITE_DONE)
	{}

	StmtIterator& operator++()
	{
		m_Status = sqlite3_step(m_Stmt.m_Stmt);

		return *this;
	}

	void operator++(int)
	{
		m_Status = sqlite3_step(m_Stmt.m_Stmt);
	}

	Stmt& operator*() { return { m_Stmt }; }

	Stmt* operator->() { return { &m_Stmt }; }

private:
	Stmt m_Stmt{ nullptr };
	int m_Status{ SQLITE_DONE };

	friend bool operator!=(const StmtIterator&, const StmtIterator&);
};

bool operator!=(const StmtIterator& it1, const StmtIterator& it2)
{
	return it1.m_Status != it2.m_Status;
}

int main()
{
	try
	{
		sqlite3* db = nullptr;

		if (SQLITE_OK != sqlite3_open("E:/SQLite3/Directory.db", &db))
			throw std::exception(sqlite3_errmsg(db));

		/*
		const char* query =
			"select p.Name, p.Value from Parameters p "
			"where p.Name = \"goforward\";";
		*/

		const char* query =
			"select c.Id, c.FirstName, c.LastName from Contact c "
			"where c.FirstName like \"A%\"";

		std::vector<Contact> contactList;

		/*
		sqlite3_exec(
			db,
			query,
			callback,
			&contactList,
			&errormsg);

		for (const Contact& contact : contactList)
			std::cout << contact.FirstName << ' ' << contact.LastName << std::endl;
		*/

		/////////////
		// Old way //
		/////////////
		sqlite3_stmt* statement{ nullptr };

		int ret = sqlite3_prepare_v2(
			db,
			query,
			-1,
			&statement,
			nullptr);
		
		if (SQLITE_OK != ret)
			throw std::exception(sqlite3_errmsg(db));

		StmtIterator it{ statement }, end{ };

		while(++it != end)
		{
			contactList.emplace_back(
				it->GetColumnValue<int>(0),
				it->GetColumnValue<const char*>(1),
				it->GetColumnValue<const char*>(2));

			std::cout << contactList.back() << std::endl;
		}

		if (SQLITE_OK != sqlite3_finalize(statement))
			throw std::exception(sqlite3_errmsg(db));

		sqlite3_close(db);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}	

	std::cin.get();
}