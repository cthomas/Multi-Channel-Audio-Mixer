#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <pthread.h>

/**
 * @class Mutex
 * @brief Class that implements a mutually exclusive locking/unlocking mechanism
 */
class Mutex
{
private:
	pthread_mutex_t _mutex;
public:
	Mutex();
	virtual ~Mutex();

	bool lock();
	bool unlock();
	pthread_mutex_t *getMutex();
};

#endif
