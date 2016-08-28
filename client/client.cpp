
#include "client.h"

static client clt;
static udp_data in_data;

string get_str()
{
	string str_number;//qq号
	string passwd;    //密码
	cout<<"please enter qq number :";
	cin>>str_number;
	cout<<"please enter qq passwd :";
	cin>>passwd;
	string tmp=" ";
	return str_number+tmp+passwd;
}

int main()
{
	char select;
	
	while(1)
	{
		show_list();
		cout<<"please select ernter :";
		select=getchar();
		switch(select)
		{
			case '1':
				//注册
				{
					string key=get_str();
					if(clt.register_send_msg(key))
						cout<<"注册成功"<<endl;
					else
						cout<<"注册失败"<<endl;
				}
				break;
			case '2':
				//登录
				{
					string key=get_str();
					if(clt.login(key))
					{
						system("clear");
						cout<<"登录成功"<<endl;
						
						string nick_name;
						string school;
						cout<<"please enter nick name :";
						fflush(stdout);
						cin>>nick_name;
						cout<<"please enter school :";
						fflush(stdout);
						sin>>school;
						
						in_data.set_nick_name(nick_name);
						in_data.set_school(school);

						pthread_t header,output,list,input;
						pthread_create(&header,NNULL,header_run,NULL);
						pthread_create(&output,NULL,output_run,NULL);
						pthread_create(&list,NULL,list_run,NULL);
						pthread_create(&input,NULL,input_run,NULL);
						//等待线程退出
						pthread_join(header,NULL);
						pthread_join(output,NULL);
						pthread_join(list,NULL);
						pthread_join(input,NULL);
					}
					else
						cout<<"登录失败"<<endl;
				}
				break;
			case '3':
				//注销qq号
				{
					string key=get_str();
					if(clt.logout(key))
						cout<<"注销成功"<<endl;
					else
						cout<<"注销失败"<<endl;
				}
				break;
			case '0':
				//退出程序
				{
					cout<<"[ ***欢迎使用*** ]"<<endl;
					exit(1);
				}
			default:
				break;
		}
		getchar();
	}
	return 0;
}
