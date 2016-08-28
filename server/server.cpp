
#include "server.h"

static server ser;

void *consumer_run(void *arg)
{
	while(true)
	{
		print_log("consumer run ....");
		ser.broadcast_msg();
	}
	return NULL;
}

void *producter_run(void *arg)
{
	while(true)
	{
		print_log("product run ....");
		ser.recv_msg();
	}
}
int main(int argc,char *argv[])
{
	//精灵进程
	daemon(0,0);
	pthread_t consumer,producter;
	//同时创建两个线程
	//一个用于生产消息，另一个用于接受消息
	pthread_create(&consumer,NULL,consumer_run,NULL);
	pthread_create(&producter,NULL,prodcuter_run,NULL);

	//主线程等待其他线程退出
	pthread_join(consumer,NULL);
	pthread_join(producter,NULL);
	
	return 0;
}
