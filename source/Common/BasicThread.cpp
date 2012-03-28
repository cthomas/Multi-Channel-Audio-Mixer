#include "BasicThread.h"
#include "DebugMessage.h"

/**
* @brief BasicThread constructor
*
* @param stack_size
*/
BasicThread::BasicThread(size_t stack_size)
{
	_shutdown = false;
	pthread_attr_init(&_attr);
	pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setstacksize(&_attr, stack_size);
}

/**
* @brief BasicThread destructor
*/
BasicThread::~BasicThread()
{
	printf("Thread [%s] shutting down...\n", _thread_str.c_str());
	signalShutdown();
	pthread_join(_pthread, NULL);

	printf("Thread [%s] shutting down...COMPLETE\n", _thread_str.c_str());
	pthread_attr_destroy(&_attr);
}

/**
* @brief Start a thread using the function thread_main passing it parameter data
*
* @param thread_main The function that will be run by the spawned thread
* @param data The data pointer to pass to the thread function
*
* @return true on successful thread creation, false otherwise
*/
bool BasicThread::start(void *(*thread_main)(void*), void *data)
{
	bool ret = true;

	if(0 != pthread_create(&_pthread, &_attr, thread_main, data))
	{
		ret = false;
	}

	return ret;
}

/**
* @brief Function that signals the thread to shut down
*/
void BasicThread::signalShutdown()
{
	if(_thread_mutex.lock())
	{
		_shutdown = true;
		_thread_mutex.unlock();
	}
}

/**
* @brief Interface to get whether a shutdown has been triggered
*
* @return true if shutdown has been triggered, false otherwise
*/
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

/**
* @brief Set a human-readable identifier for the BasicThread
*
* @param id_str The string to associate with this BasicThread
*/
void BasicThread::setThreadIdentifier(const std::string & id_str)
{
	if(_thread_mutex.lock())
	{
		_thread_str = id_str;
		_thread_mutex.unlock();
	}
}
