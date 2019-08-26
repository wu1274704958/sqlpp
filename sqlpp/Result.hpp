#pragma once

#include <mysql.h>
#include "exception.h"
#include <tuple>
#include <serialization.hpp>

namespace sql {
	
	class Row
	{
	public:
		Row() : m_row(nullptr), len(0)
		{

		}
		Row(::MYSQL_ROW row,int len) : m_row(row),len(len)
		{

		}
		operator bool()
		{
			return m_row != nullptr;
		}
		template<size_t I,typename TUP,typename F,typename ... Args>
		void get_tup_sub(TUP& tup)
		{
			if constexpr (std::is_same_v<std::string, std::remove_cv_t<F>>)
			{
				std::get<I>(tup) = m_row[I];
			}
			else if constexpr(wws::is_std_list<F>::val){
				F f;
				std::string str(m_row[I]);
				wws::parser_stl(str, f);
				std::get<I>(tup) = std::move(f);
			}
			else {
				std::string str(m_row[I]);
				std::get<I>(tup) = wws::parser<F>(str);
			}
			if constexpr (sizeof...(Args) > 0)
			{
				get_tup_sub<I + 1,TUP, Args...>(tup);
			}
		}
		template<typename ...TS>
		std::tuple<TS...> get_tup()
		{
			assert(sizeof...(TS) <= len);
			std::tuple<TS...> tup;

			get_tup_sub<0,std::remove_cv_t<decltype(tup)>,TS...>(tup);
			return tup;
		}


		template<size_t I, typename CLS, typename F, typename ... Args>
		void get_sub(CLS& cls,F CLS::* fir,Args CLS::* ... args)
		{
			if constexpr (std::is_same_v<std::string, std::remove_cv_t<F>>)
			{
				cls.*fir = m_row[I];
			}
			else if constexpr (wws::is_std_list<F>::val) {
				F f;
				std::string str(m_row[I]);
				wws::parser_stl(str, f);
				cls.*fir = std::move(f);
			}
			else {
				std::string str(m_row[I]);
				cls.*fir = wws::parser<F>(str);
			}
			if constexpr (sizeof...(Args) > 0)
			{
				get_sub<I + 1, CLS, Args...>(cls,args...);
			}
		}
		template<typename CLS,typename ...TS>
		CLS get(TS CLS::* ...ts)
		{
			assert(sizeof...(TS) <= len);
			CLS cls;

			get_sub<0, CLS, TS...>(cls,ts ...);
			return cls;
		}
	private:
		::MYSQL_ROW m_row;
		int len;
	};

	class Result {
	public:
		Result(::MYSQL_RES* pr) : m_res(pr)
		{

		}
		Result(const Result&) = delete;
		Result& operator=(const Result&) = delete;
		Result(Result&& t)
		{
			m_res = t.m_res;
			t.m_res = nullptr;
		}
		Result& operator=(Result&& t)
		{
			free();
			m_res = t.m_res;
			t.m_res = nullptr;
			return *this;
		}
		operator bool()
		{
			return m_res != nullptr;
		}
		
		Row next()
		{
			return Row(::mysql_fetch_row(m_res),::mysql_num_fields(m_res));
		}
		~Result()
		{
			free();
		}
	private:
		void free()
		{
			if(m_res)
				::mysql_free_result(m_res);
		}
		::MYSQL_RES* m_res;
	};
}