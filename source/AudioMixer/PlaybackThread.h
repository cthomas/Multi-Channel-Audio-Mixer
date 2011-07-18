#ifndef _PLAYBACK_THREAD_H_
#define _PLAYBACK_THREAD_H_

#include "BasicThread.h"
#include "MultiChannelMixer.h"

class PlaybackThread
	  : public MultiChannelMixer,
		public BasicThread
{
private:
	PlaybackThread();
	static void *threadMain(void *data);

public:
	virtual ~PlaybackThread();
	static PlaybackThread *startPlaybackThread();
};

#endif
