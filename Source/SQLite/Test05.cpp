#include <iostream>
#include <memory>

#include <Utilities/External/sqlite3/sqlite3.h>

int callback(void* a_param, int argc, char** argv, char** column)
{
	for (int i = 0; i < argc; ++i)
		std::cout << argv[i] << ' ';

	std::cout << std::endl;

	return 0;
}

int main()
{
	try
	{
		sqlite3* _db;

		if (SQLITE_OK != sqlite3_open("../Console3D/Config.db", &_db))
			throw std::exception(sqlite3_errmsg(_db));

		std::shared_ptr<sqlite3> db(_db, [](sqlite3* ptr) { sqlite3_close(ptr); });

		const char* query =
			"select p.Name, p.Value from Parameters p "
			"where p.Name = \"goforward\";";

		char* errormsg = nullptr;

		sqlite3_exec(_db, query, callback, nullptr, &errormsg);

		if (errormsg)
		{
			std::cout << errormsg << std::endl;
			sqlite3_free(errormsg);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}