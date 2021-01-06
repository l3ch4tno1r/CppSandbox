#pragma once

#include <exception>

#include <Utilities/External/sqlite3/sqlite3.h>

namespace LCN {
	namespace SQLiteEntity {

		class Query;

		class DataBase
		{
		public:
			DataBase(const char* dbname);

			virtual ~DataBase();

			void ExecuteQuery(const Query& query);

		private:
			sqlite3* m_DB = nullptr;
		};
	}
}