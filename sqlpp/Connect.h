#pragma once
#include <mysql.h>
#include <string>
#include "Result.hpp"

namespace sql {

	class Drive;
	class Connect {
	public:
		friend class Drive;
		Result query(const char *str);

		Result query(std::string&& str)
		{
			return query(str.data());
		}
		Result query(std::string& str)
		{
			return query(str.data());
		}
		unsigned long long affected_rows();

		template<typename ...Args>
		Result insert(const char *tab_n, Args&& ...args)
		{
			std::string str = "insert "; 
			str += tab_n;
			str += " values(";
			append(str, std::forward<Args>(args)...);
			str += ");";
			return query(str);
		}

		int ping();

	private:

		template<typename F,typename ...Args>
		void append(std::string& str, F&& f, Args&&...args)
		{
			bool end = sizeof...(Args) == 0;
			if constexpr (std::is_same_v<std::string, std::remove_cv_t<F>>)
			{
				str += '\'';
				str += f;
				str += '\'';
			}else
			if constexpr ( std::is_same_v<const char,std::remove_extent_t<std::remove_reference_t<F>>>  )
			{
				str += '\'';
				str += f;
				str += '\'';
			}
			else if constexpr (wws::is_std_list<F>::val) {
				str += wws::to_string(std::forward<F>(f));
			}
			else {
				str += wws::to_string(std::forward<F>(f));
			}
			if (!end) str += ',';
			if constexpr (sizeof...(Args) > 0)
			{
				append(str, std::forward<Args>(args)...);
			}
		}

		Connect(
			Drive& d,
			const char *host,
			const char *user,
			const char *passwd,
			const char *db,
			unsigned int port);
		Drive& dri;
	};

}