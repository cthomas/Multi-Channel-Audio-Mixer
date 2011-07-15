#ifndef _PLAYBACK_THREAD_H_
#define _PLAYBACK_THREAD_H_

#include <pthread.h>
#include "Mutex.h"

class PlaybackThread
{
private:
	PlaybackThread();

	pthread_t _pthread;
	unsigned int _thread_id;
	pthread_attr_t _attr;
	bool _shutdown;
	Mutex _mutex;

	void signalShutdown();
	bool shutdown();
	static void *threadMain(void *data);

public:
	~PlaybackThread();
	static PlaybackThread *startPlaybackThread();
	void stopPlaybackThread();
};

#endif
