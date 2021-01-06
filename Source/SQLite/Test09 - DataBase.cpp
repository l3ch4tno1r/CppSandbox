#include <iostream>

#include "SQLiteEntity/DataBase.h"

int main()
{
	try
	{
		LCN::SQLiteEntity::DataBase db("E:/SQLite3/Directory.db");
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}