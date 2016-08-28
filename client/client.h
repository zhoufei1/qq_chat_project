
#include "../comm/comm.h"

class client
{
	public:
		client(const string &ip="127.0.0.1",const ssize_t port=8080)
			:_sock(-1),_ip(ip),_port(port)
		{
			init();
		}
		~client()
		{
			if(_sock !=-1)
				close(_sock);
		}
	private:
		void init()
		{
			_sock=socket(AF_INET,SOCK_DGRAM,0);
			assert(_sock !=-1);
		}
	public:
		//注册
		bool register_send_msg(string &_msg)
		{
			string msg="REGISTER:";
			msg+=_msg;
			return send_msg(msg);
		}
		//登录
		bool login(string &_msg)
		{
			string msg="LOGIN:";
			msg+=_msg;
			return send_msg(msg);
		}
		//注销
		bool logout(string &_msg)
		{
			string msg="LOGOUT:";
			msg+=_msg;
			return send_msg(msg);
		}
	public:
		bool send_msg(string &msg)
		{
			struct sockaddr_in server_addr;
			socklen_t server_len=sizeof(server_addr);
			bzero(&server_addr,server_len);

			server_addr.sin_family=AF_INET;
			server_addr.sin_port=htons(_port);
			server_addr.sin_addr.s_addr=inet_addr(_ip.c_str());

			ssize_t ret=sendto(_sock,msg.c_str(),msg.size(),0,\
					(struct sockaddr*)&server_addr,server_len);
			if(ret ==-1)
				return false;
			else
				return true;
		}
		ssize_t recv_msg(string &msg)
		{
			char buf[BUF_SIZE];
			memset(buf,'\0',BUF_SIZE);

			struct sockaddr_in dest;
			socklen_t len=sizeof(dest);
			bzero(&dest,len);

			ssize_t size=recvfrom(_sock,buf,BUF_SIZE,0,(struct sockaddr*)&dest,&len);
			if(-1 == size)
				perror("client get msg failed");
			else
				msg=buf;
			return size;
		}

	private:
		vector<string>::iterator Find(string &key)
		{
			return find(friend_list.begin(),friend_list.end(),key);
		}
	public:
		bool is_friend_exist(string &key)
		{
			vector<string>::iterator it=Find(key);
			if(it !=friend_list.end())
				return true;
			else
				return false;
		}
		void add_friend(string &key)
		{
			if( !is_friend_exist(key))
				friend_list.push_back(key);
		}
		void del_friend(string &key)
		{
			vector<string>::iterator it=Find(key);
			if(it !=friend_list.end())
				it=friend_list.erase(it);
		}
	private:
		int _sock;
		string _ip;
		size_t _port;
		vector<string> friend_list;
};

void inline show_list()
{
	cout<<"************************"<<endl;
	cout<<"**[1] 注册  [2] 登录 ***"<<endl;
	cout<<"**[3] 注销  [0] 退出 ***"<<endl;
	cout<<"************************"<<endl;
}
