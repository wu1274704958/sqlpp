#include "sqlpp/Drive.h"
#include "sqlpp/exception.h"
#include "sqlpp/Connect.h"

namespace sql {

	Drive::Drive()
	{
		if (HasOne)
		{
			throw SqlException("already has one!");
		}
		pmysql = ::mysql_init(nullptr);
		if (pmysql == nullptr)
		{
			throw SqlException("init error!");
		}
		HasOne = true;
	}

	Connect Drive::connect(const char *host,
		const char *user,
		const char *passwd,
		const char *db,
		unsigned int port)
	{
		return Connect(*this, host, user, passwd, db, port);
	}

	Drive::~Drive()
	{
		if (pmysql)
			::mysql_close(pmysql);
		::mysql_library_end();
		pmysql = nullptr;
	}

}