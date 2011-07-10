#ifndef _PLAYBACK_THREAD_H_
#define _PLAYBACK_THREAD_H_

#include <pthread.h>

class PlaybackThread
{
private:
	PlaybackThread();

	pthread_t _pthread;
	unsigned int _thread_id;
	pthread_attr_t _attr;
	pthread_mutex_t _mutex;
	bool _shutdown;

	void signalShutdown();
	bool shutdown();
	static void *threadMain(void *data);

public:
	~PlaybackThread();
	static PlaybackThread *startPlaybackThread();
	void stopPlaybackThread();
};

#endif
