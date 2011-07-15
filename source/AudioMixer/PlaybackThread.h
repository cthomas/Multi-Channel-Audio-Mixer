#ifndef _PLAYBACK_THREAD_H_
#define _PLAYBACK_THREAD_H_

#include <pthread.h>
#include "PlaybackInterface.h"
#include "Mutex.h"

class PlaybackThread
	: public PlaybackInterface
{
private:
	PlaybackThread();

	pthread_t _pthread;
	unsigned int _thread_id;
	pthread_attr_t _attr;
	bool _shutdown;
	Mutex _mutex;
	PlaybackInterface::PLAYBACK_INTERFACE_STATUS _playback_status;
	AudioChannelInterface *_playback_channel;

	void signalShutdown();
	bool shutdown();
	static void *threadMain(void *data);

public:
	~PlaybackThread();
	static PlaybackThread *startPlaybackThread();
	void stopPlaybackThread();

public: //PlaybackInterface
	void setPlaybackChannel(AudioChannelInterface *channel);
	const PLAYBACK_INTERFACE_STATUS getStatus();

};

#endif
