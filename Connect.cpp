#include "Connect.h"
#include "Drive.h"
#include "exception.h"
#include "Result.hpp"

namespace sql {
	Result Connect::query(const char *str)
	{
		int ret = ::mysql_query(dri.pmysql, str);
		if (ret != 0)
		{
			throw SqlException(::mysql_error(dri.pmysql));
		}
		return Result(::mysql_store_result(dri.pmysql));
	}

	unsigned long long Connect::affected_rows()
	{
		return ::mysql_affected_rows(dri.pmysql);
	}

	Connect::Connect(
		Drive& d,
		const char *host,
		const char *user,
		const char *passwd,
		const char *db,
		unsigned int port) : dri(d)
	{
		::MYSQL* ret = ::mysql_real_connect(dri.pmysql, host, user, passwd, db, port, nullptr, 0);
		if (ret == nullptr)
		{
			throw SqlException(::mysql_error(dri.pmysql));
		}
	}
}