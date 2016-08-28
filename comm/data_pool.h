
#pragma once

#include <iostream>
#include <semaphore.h>
#include <string>
#include <vector>
using namespace std;

const int POOL_SIZE=512;

class data_pool
{
	public:
		data_pool(int size=POOL_SIZE):pool_size(size)
		{
			start=end=0;
			sem_init(&product_sem,0,size);
			sem_init(&consumer_sem,0,size);
		}
		~data_pool()
		{
			sem_destroy(&product_sem);
			sem_destroy(&sonsumer_sem);
		}
	public:
		void get_msg(string &msg)
		{
			sem_wait(&consumer_sem);
			msg=pool[end];
			sem_post(&product_sem);
			end=(++end)%pool_size;
		}
		void put_msg(string &msg)
		{
			sem_wait(&product_sem);
			pool[start]=msg;
			sem_post(&consumer_sem);
			start=(++start)%pool_size;
		}
	private:
		const int pool_size;
		vector<string> pool;
		sem_t product_sem;
		sem_t consumer_sem;

		int start;
		int end;
};
