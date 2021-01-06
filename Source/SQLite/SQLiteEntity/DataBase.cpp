#include "DataBase.h"

namespace LCN {
	namespace SQLiteEntity {

		DataBase::DataBase(const char * dbname)
		{
			if (SQLITE_OK != sqlite3_open(dbname, &m_DB))
				throw std::exception(sqlite3_errmsg(m_DB));
		}

		DataBase::~DataBase()
		{
			sqlite3_close(m_DB);
		}
	}
}