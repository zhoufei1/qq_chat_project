
#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#include <string.h>
#include <strings.h> //bzero
using namespace std;

#define BUF_SIZE 1024


void print_log(const char *msg)
{
	assert(msg);
	cout<<msg<<endl;
}
