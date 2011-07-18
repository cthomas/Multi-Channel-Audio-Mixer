#include "BasicThread.h"
#include "DebugMessage.h"

BasicThread::BasicThread(size_t stack_size)
{
	_shutdown = false;
	pthread_attr_init(&_attr);
	pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setstacksize(&_attr, stack_size);
}

BasicThread::~BasicThread()
{
	printf("Thread [%s] shutting down...\n", _thread_str.c_str());
	signalShutdown();
	pthread_join(_pthread, NULL);

	printf("Thread [%s] shutting down...COMPLETE\n", _thread_str.c_str());
	pthread_attr_destroy(&_attr);
}

bool BasicThread::start(void *(*thread_main)(void*), void *data)
{
	bool ret = true;

	if(0 != pthread_create(&_pthread, &_attr, thread_main, data))
	{
		ret = false;
	}

	return ret;
}

void BasicThread::signalShutdown()
{
	if(_thread_mutex.lock())
	{
		_shutdown = true;
		_thread_mutex.unlock();
	}
}

bool BasicThread::shutdown()
{
	bool ret = false;

	if(_thread_mutex.lock())
	{
		ret = _shutdown;
		_thread_mutex.unlock();
	}

	return ret;
}

void BasicThread::setThreadIdentifier(const std::string & id_str)
{
	if(_thread_mutex.lock())
	{
		_thread_str = id_str;
		_thread_mutex.unlock();
	}
}
