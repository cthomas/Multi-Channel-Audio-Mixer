
#include "Mutex.h"

/**
* @brief Mutex constructor
*/
Mutex::Mutex()
{
	pthread_mutex_init(&_mutex, NULL);
}

/**
* @brief Mutex destructor
*/
Mutex::~Mutex()
{
	pthread_mutex_destroy(&_mutex);
}

/**
* @brief Obtain an exclusive lock on the Mutex
*
* @return true on success, false otherwise
*/
bool Mutex::lock()
{
	return (0 == pthread_mutex_lock(&_mutex));
}

/**
* @brief Release a previous lock on the Mutex
*
* @return true on success, false otherwise
*/
bool Mutex::unlock()
{
	return (0 == pthread_mutex_unlock(&_mutex));
}

/**
* @brief Get a pointer to the pthread_mutex_t utilized by the Mutex
*
* @return A pointer to the pthread_mutex_t utilized by the Mutex
*/
pthread_mutex_t *Mutex::getMutex()
{
	return &_mutex;
}

