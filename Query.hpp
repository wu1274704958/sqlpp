#pragma once
#include "Connect.h"
#include <comm.hpp>

#define DEF_KEY_WORD_VAL_SAME(C) struct C { constexpr static const char * _val = #C; }
#define DEF_KEY_WORD_VAL(C,V) struct C { constexpr static const char * _val = #V; };
#define DEF_KEY_WORD_VAL_S(C,S) struct C { constexpr static const char * _val = S; };

namespace sql {
	namespace  K
	{
		struct none { constexpr static const char * _val = ""; };
		DEF_KEY_WORD_VAL_SAME(select);
		DEF_KEY_WORD_VAL_SAME(from);
		DEF_KEY_WORD_VAL(star,*);
		DEF_KEY_WORD_VAL_SAME(create);
		DEF_KEY_WORD_VAL_SAME(table);
		DEF_KEY_WORD_VAL(primary_key,primary key);
		DEF_KEY_WORD_VAL(auto_increment, auto increment);
		DEF_KEY_WORD_VAL(not_null,not null);
		DEF_KEY_WORD_VAL(semicolon,;);
		DEF_KEY_WORD_VAL_S(l_bar,"(");
		DEF_KEY_WORD_VAL_S(r_bar,")");
		DEF_KEY_WORD_VAL(Int,int);
		DEF_KEY_WORD_VAL_SAME(varchar);
		DEF_KEY_WORD_VAL(Char,char);
		DEF_KEY_WORD_VAL_SAME(values);
		DEF_KEY_WORD_VAL_SAME(drop);
		DEF_KEY_WORD_VAL(Where,where);
		DEF_KEY_WORD_VAL(eq,=);
		DEF_KEY_WORD_VAL(neq,<>);
		DEF_KEY_WORD_VAL(gt,> );
		DEF_KEY_WORD_VAL(lt	,< );
		DEF_KEY_WORD_VAL(ge	,>=);
		DEF_KEY_WORD_VAL(le	,<=);
		DEF_KEY_WORD_VAL_SAME(between);
		DEF_KEY_WORD_VAL_SAME(like);
		DEF_KEY_WORD_VAL(And,and);
		DEF_KEY_WORD_VAL_SAME(update);
		DEF_KEY_WORD_VAL_SAME(set);
	};

	template <class T>											
	using has_key_word_val_t = decltype(T::_val);				
																
	template <typename T>										
	using has_key_word_val_vt = wws::is_detected<has_key_word_val_t,T>;

	class Query
	{
	public:
		Query() = default;
		~Query() = default;


		template<typename KW>
		constexpr static const char* get_key_word()
		{
			static_assert(has_key_word_val_vt<KW>::value,"This type not KeyWord type!!!");
			return KW::_val;
		}
		template<typename KW,bool append_space = true, bool append_qm = false,char Token = ' ', char Token2 = ' '>
		Query& a(std::string&& app) {
			data += get_key_word<KW>();
			if constexpr (append_space)
			{
				data += Token;
			}
			if constexpr (append_qm)
				data += '\'';
			data += app;
			if constexpr (append_qm)
				data += '\'';
			if constexpr (append_space)
			{
				data += Token2;
			}
			return *this;
		}

		template<typename KW, bool append_space = true, char Token = ' '>
		Query& a() {
			data += get_key_word<KW>();
			if constexpr (append_space)
			{
				data += Token;
			}
			return *this;
		}

		template<bool append_space = true,char Token = ' ', bool append_qm = false>
		Query& a(std::string&& app) {
			if constexpr (append_qm)
				data += '\'';
			data += app;
			if constexpr (append_qm)
				data += '\'';
			if constexpr (append_space)
			{
				data += Token;
			}
			return *this;
		}

		Query& select(std::initializer_list<const char *> fields) {
			this->a<K::select>();
			for (auto it = fields.begin(); it != fields.end(); )
			{
				data += (*it);
				++it;
				if (it != fields.end())
					data += ',';
			}
			data += ' ';			
			return *this;
		}

		decltype(auto) exec(Connect& c)
		{
			return c.query(data);
		}

		void clear()
		{
			data.clear();
		}
		

	private:
		std::string data;
	};
}

#undef DEF_KEY_WORD_VAL_SAME
#undef DEF_KEY_WORD_VAL
#undef DEF_KEY_WORD_VAL_S