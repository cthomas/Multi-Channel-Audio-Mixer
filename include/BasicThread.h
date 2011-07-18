#ifndef _BASIC_THREAD_H_
#define _BASIC_THREAD_H_

#include <pthread.h>
#include "Mutex.h"

class BasicThread
{
protected:
	pthread_t _pthread;
	unsigned int _thread_id;
	pthread_attr_t _attr;
	bool _shutdown;
	Mutex _thread_mutex;

	virtual void signalShutdown();
	virtual bool shutdown();
	BasicThread(size_t stack_size);
	virtual ~BasicThread();
public:
	virtual bool start(void *(*thread_main)(void*), void *data);
};
#endif
