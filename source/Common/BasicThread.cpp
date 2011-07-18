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
	TRACE("Thread shutting down...\n");
	signalShutdown();
	pthread_join(_pthread, NULL);

	TRACE("Thread shutting down...COMPLETE\n");
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
	if(_mutex.lock())
	{
		_shutdown = true;
		_mutex.unlock();
	}
}

bool BasicThread::shutdown()
{
	bool ret = false;

	if(_mutex.lock())
	{
		ret = _shutdown;
		_mutex.unlock();
	}

	return ret;
}

