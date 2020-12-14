#include <iostream>
#include <thread>
#include <atomic>

#include <Utilities/External/sqlite3/sqlite3.h>

using namespace std::literals::chrono_literals;

const char* dbpath = "E:\\Utilisateurs\\Arnaud\\Documents\\GitHub\\Console3D\\Console3D\\Config.db";

std::atomic_bool run = true;

int callback(void* a_param, int argc, char** argv, char** column)
{
	std::cout << std::this_thread::get_id() << '\t';

	for (int i = 0; i < argc; ++i)
		std::cout << argv[i] << ' ';

	std::cout << std::endl;

	return 0;
}

void AccessingThread()
{
	sqlite3* db;

	try
	{
		while (run)
		{
			if (sqlite3_open(dbpath, &db))
				throw std::exception(sqlite3_errmsg(db));

			const char* query = "select p.Name, p.Value from Parameters p";
			char* errormsg = nullptr;

			sqlite3_exec(db, query, callback, nullptr, &errormsg);

			if (errormsg)
			{
				std::cout << errormsg << std::endl;
				sqlite3_free(errormsg);
			}

			//std::this_thread::sleep_for(1s);

			sqlite3_close(db);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}
}

int main()
{
	std::thread workers[] = {
		std::thread(AccessingThread),
		std::thread(AccessingThread),
		std::thread(AccessingThread),
		std::thread(AccessingThread)
	};

	std::cin.get();

	run = false;

	std::cout << "main done." << std::endl;

	for (std::thread& w : workers)
		w.join();

	std::cin.get();
}