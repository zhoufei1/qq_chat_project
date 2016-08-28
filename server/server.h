
#include "../comm/comm.h"
static string SERVER_IP="127.0.0.1";

class server
{
	public:
		server(string &ip=SERVER_IP,size_t port=8080)
			:sock(-1),host_ip(ip),host_port(port)
		{
			init();
		}
		~server()
		{
			if(-1 !=sock)
				close(sock);
		}
	private:
		void init()
		{
			sock=socket(AF_INET,SOCK_DGRAM,0);
			assert(sock !=-1);
			
			struct sockaddr_in local;
			socklen_t len=sizeof(local);

			bzero(&local,len);
			local.sin_family=AF_INET;
			local.sin_port=htons(host_port);
			local.sin_addr.s_addr=inet_addr(host_ip.c_str());

			if(-1 == bind(sock,(struct sockaddr*)&local,len))
				perror("server bind failed");
		}
	private:
		void comm_recv_msg(string &msg,string &number,string &passwd)
		{
			const int SIZE=512;
			char buf[SIZE];
			memset(buf,'\0',SIZE);
			for(int i=0; i<msg.size(); ++i)
			{
				if(msg[i] == ' ')
				{
					strncpy(buf,msg.c_str(),i+1);
					number=buf;
					passwd=msg.c_str()+i+1;
					return;
				}
			}
		}
	private:
		void register_recv_msg(string &msg)
		{
			string number;
			string passwd;
			comm_recv_msg(msg,number,passwd);
			map<string,string>::iterator it=user.find(number);
			//不存在，插入
			if(it == user.end())
			{
				cout<<"qq :"<<number<<"\tpasswd:"<<passwd<<endl;
				cout<<"注册成功"<<endl;
				user[number]=passwd;
				//插入到数据里
				//insert_mysql(number,passwd);
			}
		}
		void login_recv_msg(string &msg)
		{
			string number;
			string passwd;
			comm_recv_msg(msg,number,passwd);
			map<string,string>::iterator it=user.find(number);
			if(it !=user.end())
			{
				cout<<"number:"<<it->first<<"\tpasswd:"<<it->second<<endl;
				//告诉客服端登录成功
				NULL;
			}
			else
				//失败
				NULL;
		}
		void logout_recv_msg(string &msg)
		{
			string number;
			string passwd;
			comm_recv_msg(msg,number,passwd);
			map<string,string>::iterator it=user.find(number);
			if(it !=user.end())
			{
				user.erase(it);
				cout<<"注销成功"<<endl;
			}
		}
		void data_recv_msg(string &msg,struct sockaddr_in &client,socklen_t len)
		{
			string log="data recv from success !";
			log+=msg;
			print_log(log.c_str());
			//把数据投放到数据池里
			msg_pool.put_msg(msg);
			//添加在线人数
			analysis_client(client,len,msg);
		}
	private:
		void analysis_client(struct sockaddr_in &client,socklen_t len,string &msg)
		{
			string ip_key=inet_ntoa(client.sin_addr);
			map<string,struct sockaddr_in>::iterator it=online_user.find(ip_key);

			if(it == online_user.end())
			{
				online_user.insert(pair<string,struct sockaddr_in>)(ip_key,client);
				string log="get a new client";
				log+=ip_key;
				print_log(log.c_str());
			}else
			{
				//进行序列化
				udp_data data;
				data.str_to_val(msg);
				string type=data.get_type();
				string cmd=data.get_cmd();
				if(type == "CMD" && cmd =="QUIT")
					online_user.erase(it);
			}
		}
		void carry_recv_msg(string buf,ssize_t size,struct sockaddr_in &client,socklen_t length)
		{
			int len=0;
			if( !strncmp("REGISTER:",buf.c_str(),len=strlen("REGISTER:")))
			{
				//注册
				buf=buf.c_str()+len;
				register_recv_msg(buf);
			}else if(!strncmp("LOGIN:",buf.c_str(),len=strlen("LOGIN:")))
			{
				//登录
				buf=buf.c_str()+len;
				login_recv_msg(buf);
			}else if(!strncmp("LOGOUT:",buf.c_str(),len=strlen("LOGOUT:")))
			{
				//注销
				buf=buf.c_str()+len;
				logout_recv_msg(buf);
			}else
			{
				//客服端向服务器发送的实体消息
				data_recv_msg(buf,client,length);
			}
		}
	public:
		ssize_t recv_msg()
		{
			char buf[BUF_SIZE];
			memset(buf,'\0',sizeof(buf));

			struct sockaddr_in client;
			socklen_t len=sizeof(client);
			bzero(&client,len);
			ssize_t size=recvfrom(sock,buf,BUF_SIZE,0,\
					(struct sockaddr*)&client,&len);
			if(-1 !=size)
			{
				//表示接受数据成功
				string key=buf;
				print_log(key.c_str());
				carry_recv_msg(key,size,client,len);
			}
			else
				perror("server recvfrom failed");
		}
		void broadcast_msg()
		{
			string msg_out;
			//拿出消息
			msg_pool.get_msg(msg_out);

			map<string,struct sockaddr_in>::iterator it=online_user.begin();
			for(; it !=online_user.end(); ++it)
			{
				socklen_t len=sizeof(it->second);
				ssize_t ret=send_msg(it->second,len,msg_out);
				if(ret <0)
				{
					//表示没有发送成功
					sleep(1);
					send_msg(it->second,len,msg_out);
				}
			}
		}
		ssize_t send_msg(struct sockaddr_in &client,socklen_t len,string &msg)
		{
			ssize_t size=sendto(sock,msg.c_str(),msg.size(),0,(struct sockaddr*)&client,len);
			if(size == -1)
				print_log("send data to client failed");
			else
				print_log("send data to client success");
			return size;
		}

	private:
		int sock;
		string host_ip;
		size_t host_port;
		map<string,string> user;//注册QQ号人数
		map<string,struct sockaddr_in> online_user;//qq在线人数
		data_pool msg_pool;  //数据池
};
