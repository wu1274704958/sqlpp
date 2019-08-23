#pragma once
#include <mysql.h>

namespace sql {

	class Connect;

	class Drive {
	public:
		friend class Connect;
		Drive() noexcept(false);

		Connect connect(const char *host,
			const char *user,
			const char *passwd,
			const char *db,
			unsigned int port);
		~Drive();

		Drive(const Drive&) = delete;
		Drive(Drive&&) = delete;
		Drive& operator=(const Drive&) = delete;
		Drive& operator=(Drive&&) = delete;

	private:
		inline static bool HasOne = false;
		::MYSQL* pmysql = nullptr;
	};

}