#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <pthread.h>

class Mutex
{
private:
	pthread_mutex_t _mutex;
public:
	Mutex();
	virtual ~Mutex();

	bool lock();
	bool unlock();
};

#endif
