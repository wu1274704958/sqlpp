#include "mysql.hpp"
#include <iostream>
#include<stdio.h>
#include <string.h>


using namespace std;

extern "C"  {
	void fff(const char *p, ...);
}
void fff(const char *p, ...) {
	int len = strlen(p);
	va_list args;
	va_start(args, p);
	for (int i = 0; i < len; ++i)
	{
		switch (p[i])
		{
		case 'a':
		{
			int a = va_arg(args, int);
			printf("int %d\n", a);
		}
		break;
		case 'b':
		{
			char a = va_arg(args, char);
			printf("char %c\n", a);
		}
		break;
		case 'c':
		{
			long long a = va_arg(args, long long);
			printf("long long %ld\n", a);
		}
		break;
		}
	}
	va_end(args);
}

using namespace sql;

struct user {
public:
	int id;
	std::string name;
	std::string	sex;
	int age;
	std::string acc;
	std::string psd;
	std::string head;
};

int main()
{
	try {

		Drive dri;

		Connect conn = dri.connect("localhost", "root", "As147258369", "test", 3306);

		Result my_result = conn.insert("user",0,"wws","wwws","psw",23,"A","213123.png");

		if (my_result)
		{

		}
		
		my_result = conn.query("SELECT * FROM user;");
		if (my_result)
		{
			Row row;
			while(row = my_result.next())
			{
				//auto [id,name,acc,psd,age, sex, head] = row.get<int,std::string,std::string,std::string,int,std::string,std::string>();
				//std::cout << id << ' ' << name << ' ' << sex << ' ' << age << ' ' << acc << ' ' << psd << ' ' << head << '\n';

				user u = row.get(&user::id,&user::name, &user::acc, &user::psd, &user::age, &user::sex, &user::head);
				std::cout << u.id << ' ' << u.name << ' ' << u.sex << ' ' << u.age << ' ' << u.acc << ' ' << u.psd << ' ' << u.head << '\n';
			}
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	system("pause");

	return 0;
}