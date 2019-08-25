#include "mysql.hpp"
#include <iostream>
#include<stdio.h>
#include <string.h>
#include "macro.h"

using namespace std;
using namespace sql;

struct user {
public:
	int uid;
	std::string name;
	std::string	sex;
	int age;
	std::string acc;
	std::string psd;
	std::string head;

	DEF_FIELDS_B
		DEF_FIELDS_C(user, uid),
		DEF_FIELDS_C(user, name),
		DEF_FIELDS_C(user, sex),
		DEF_FIELDS_C(user, age),
		DEF_FIELDS_C(user, acc),
		DEF_FIELDS_C(user, psd),
		DEF_FIELDS_C(user, head)
	DEF_FIELDS_E
	
	DEF_GET_FIELD_NAME

	DEF_GET_CLS_NAME(user)
};

int main()
{
	cout << user::get_field_name(&user::uid) << endl;
	cout << user::get_field_name(&user::name) << endl;
	cout << user::get_field_name(&user::acc) << endl;
	cout << user::get_field_name(&user::head) << endl;
	try {
		
		Drive dri;

		Connect conn = dri.connect("localhost", "root", "As147258369", "cs", 3306);
		
		Query q;

		auto my_result = q.as<K::select,K::star,K::from>()
			.ac<user>()
			.where<K::neq, false>(&user::uid,"10005")
			.exec(conn);
		if (my_result)
		{
			Row row;
			while(row = my_result.next())
			{
				//auto [id,name,acc,psd,age, sex, head] = row.get<int,std::string,std::string,std::string,int,std::string,std::string>();
				//std::cout << id << ' ' << name << ' ' << sex << ' ' << age << ' ' << acc << ' ' << psd << ' ' << head << '\n';

				user u = row.get(&user::uid,&user::name, &user::acc, &user::psd, &user::age, &user::sex, &user::head);
				std::cout << u.uid << ' ' << u.name << ' ' << u.sex << ' ' << u.age << ' ' << u.acc << ' ' << u.psd << ' ' << u.head << '\n';
			}
		}
		q.clear();
		/*q.select({ user::get_field_name(&user::acc),user::get_field_name(&user::psd) })
			.a<K::from>(user::get_class_name())
			.a<K::Where>(user::get_field_name(&user::uid))
			.a<K::neq>("10005")
			.a<K::And>(user::get_field_name(&user::name))
			.a<K::neq,true,true>("1");
			my_result = q.exec(conn);*/

		q.select(&user::acc,&user::psd,&user::age)
			.where<K::neq, false>(&user::uid,"10005")
			.a<K::And>(&user::name)
			.a<K::neq, true, true>("1");
		my_result = q.exec(conn);

		if (my_result)
		{
			Row row;
			while (row = my_result.next())
			{
				//auto [id,name,acc,psd,age, sex, head] = row.get<int,std::string,std::string,std::string,int,std::string,std::string>();
				//std::cout << id << ' ' << name << ' ' << sex << ' ' << age << ' ' << acc << ' ' << psd << ' ' << head << '\n';

				auto [acc,psd,age] = row.get_tup<std::string,std::string,int>();
				std::cout << acc << ' ' << psd << ' '<< age <<  '\n';
			}
		}

		q.clear();
		my_result = q.asc<user, K::Delete, K::from>()
			.where<K::eq,false>(&user::uid, "10007")
			.exec(conn);

		cout << "affected " << conn.affected_rows() << endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	system("pause");

	return 0;
}