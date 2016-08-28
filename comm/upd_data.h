
#pragma once

#include <iostream>
using namespace std;
#include "my_json.h"

const int BUF_SIZE=1024;

class udp_data
{
	public:
		udp_data()
		{}
		~udp_data()
		{}
	public:
		void val_to_str(string &out)
		{
			Json::Value _val;
			_val["nick_name"]=nick_name;
			_val["school"]=school;
			_val["msg"]=msg;
			_val["type"]=type;
			_val["cmd"]=cmd;
			//序列化
			json_bit.serialize(_val,out);
		}
		void str_to_val(string &in)
		{
			Json::Value _val;
			//反序列化
			json_bit.deserialize(in,_val);
			nick_name=_val["nick_name"].asString();
			school=_val["school"].asString();
			msg=_val["msg"].asString();
			type=_val["type"].asString();
			cmd=_val["cmd"].asString();
		}
	public:
		string& get_nick_name()const
		{
			return nick_name;
		}
		string& get_school()const
		{
			return school;
		}
		string& get_msg()const
		{
			return msg;
		}
		string& get_type()const
		{
			return type;
		}
		string& get_cmd()const
		{
			return cmd;
		}
	public:
		void set_nick_name(string &_nick_name)
		{
			nick_name=_nick_name;
		}
		void set_school(string &_school)
		{
			school=_school;
		}
		void set_msg(string &_msg)
		{
			msg=_msg;
		}
		void set_type(string &_type)
		{
			type=_type;
		}
		void set_cmd(strig &_cmd)
		{
			cmd=_cmd;
		}
	private:

		my_json json_bit;
		string nick_name;
		string school;
		string msg;
		string type;
		string cmd;
};
