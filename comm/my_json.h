
#pragma once

#include <iostram>
using namespace std;
#include <json/json.h>
class my_json
{
	public:
		void serialize(Json::Value &val,string &out)
		{
#ifdef _MY_JSON_STYLE_
			Json::StyledWriter _write;
#else
			Json::FastWriter _write;
#endif
			out=_write.write(val);
		}
		int deserialize(string &str,Json::Value &val)
		{
			Json::Reader _reader;
			if(_reader.parse(str,val,false))
				return 0;
			else
				return 1;
		}
};
