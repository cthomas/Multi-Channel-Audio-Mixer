
#include "Mutex.h"

Mutex::Mutex()
{
	pthread_mutex_init(&_mutex, NULL);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&_mutex);
}

bool Mutex::lock()
{
	return (0 == pthread_mutex_lock(&_mutex));
}

bool Mutex::unlock()
{
	return (0 == pthread_mutex_unlock(&_mutex));
}
