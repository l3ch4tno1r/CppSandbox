#include <iostream>
#include <string_view>

#include <Utilities/External/sqlite3/sqlite3.h>

const char* cstr_DirectoryDB = "E:/SQLite3/Directory.db";

class DirectoryDB
{
public:
	DirectoryDB(const char* dir)
	{
		if (SQLITE_OK != sqlite3_open(dir, &m_db))
			throw std::exception(sqlite3_errmsg(m_db));
	}

	~DirectoryDB()
	{
		sqlite3_close(m_db);
	}

private:
	sqlite3* m_db{ nullptr };
};

int main()
{
	try
	{
		DirectoryDB db(cstr_DirectoryDB);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}