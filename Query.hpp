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
		DEF_KEY_WORD_VAL_SAME(auto_increment);
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
		DEF_KEY_WORD_VAL(Delete,delete);
		DEF_KEY_WORD_VAL_SAME(into);
		DEF_KEY_WORD_VAL_SAME(insert);
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

		template<typename ...KW>
		Query& as()
		{
			(a<KW>(), ...);
			return *this;
		}

		template<typename KW,typename CLS,typename F>
		Query& a(F CLS::* f)
		{
			return a<KW>().
				a(CLS::get_field_name(f));
		}

		template<typename CLS, typename F>
		Query& a(F CLS::* f)
		{
			return a(CLS::get_field_name(f));
		}

		template<typename CLS, typename ...KW>
		Query& asc()
		{
			(a<KW>(), ...);
			a(CLS::get_class_name());
			return *this;
		}

		template<typename CLS>
		Query& ac()
		{
			return a(CLS::get_class_name());
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


		template<typename CLS, typename Fir,typename ...FS>
		void select_sub(Fir CLS::*fir, FS CLS::* ...fs) {
			
			data += CLS::get_field_name(fir);
			if constexpr (sizeof...(FS) > 0)
			{
				data += ',';
				select_sub(fs...);
			}
		}

		template<typename CLS,typename ...FS>
		Query& select(FS CLS::* ...fs) {
			this->a<K::select>();
			select_sub(fs...);
			data += ' ';
			this->a<K::from>(CLS::get_class_name());
			return *this;
		}

		template<typename KW, bool append_qm,typename CLS, typename F>
		Query& where(F CLS::* f,std::string&& val)
		{
			this->a<K::Where>();
			this->a(CLS::get_field_name(f));
			this->a<KW>();
			return this->a<true, ' ', append_qm>(std::move(val));
		}

		template<typename CLS,typename ... FS>
		Query& insert(CLS& cls,FS CLS::* ...fs)
		{
			asc<CLS, K::insert, K::into>();
			a<K::l_bar>();
			insert_k_sub(fs...);
			a<K::r_bar>();
			as<K::values,K::l_bar>();
			insert_v_sub(cls, fs...);
			return a<K::r_bar>();
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
		template<typename CLS,typename F, typename ... FS>
		void insert_k_sub(F CLS::* f,FS CLS::* ...fs)
		{
			if constexpr (sizeof...(FS) > 0)
			{
				a<true, ','>(CLS::get_field_name(f));
				insert_k_sub(fs...);
			}
			else {
				a<false>(CLS::get_field_name(f));
			}
		}

		template<typename CLS, typename F, typename ... FS>
		void insert_v_sub(CLS& cls,F CLS::* f, FS CLS::* ...fs)
		{
			constexpr bool is_list = wws::is_std_list<std::remove_cv_t<F>>::val;
			constexpr bool append_qm = std::is_same_v<std::string, std::remove_cv_t<F>> || is_list;
			constexpr bool is_str = std::is_same_v<std::string, std::remove_cv_t<F>>;
			if constexpr (sizeof...(FS) > 0)
			{
				if constexpr (is_str)
				{
					std::string temp = cls.*f;
					a<true, ',', append_qm>(std::move(temp));
				}else
				{ 
					a<true, ',', append_qm>(wws::to_string(cls.*f));
				}
				insert_v_sub(cls,fs...);
			}
			else {
				if constexpr (is_str)
				{
					std::string temp = cls.*f;
					a<false, ' ', append_qm>(std::move(temp));
				}
				else
				{
					a<false, ' ', append_qm>(wws::to_string(cls.*f));
				}
			}
		}
	private:
		std::string data;
	};
}

#undef DEF_KEY_WORD_VAL_SAME
#undef DEF_KEY_WORD_VAL
#undef DEF_KEY_WORD_VAL_S