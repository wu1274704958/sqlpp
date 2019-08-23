#pragma once
#include <exception>
namespace sql {
	struct SqlException : public std::exception
	{
		SqlException(const char *msg) : exception(msg) {}
	};
}