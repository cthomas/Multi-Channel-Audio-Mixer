
#include <unistd.h>

#include "DebugMessage.h"

#include "PlaybackThread.h"

PlaybackThread::PlaybackThread()
{
	_shutdown = false;
	pthread_attr_init(&_attr);
	pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setstacksize(&_attr, 128 * 1024);
}

PlaybackThread::~PlaybackThread()
{
	TRACE("Playback thread shutting down...\n");
	signalShutdown();
	pthread_join(_pthread, NULL);

	TRACE("Playback thread shutting down...COMPLETE\n");
	pthread_attr_destroy(&_attr);
}

PlaybackThread *PlaybackThread::startPlaybackThread()
{
	PlaybackThread *playback = new PlaybackThread();

	if(playback)
	{
		if(0 != pthread_create(&playback->_pthread, &playback->_attr, PlaybackThread::threadMain, (void*)playback))
		{
			delete playback;
			playback = NULL;
		}
	}

	return playback;
}

void PlaybackThread::stopPlaybackThread()
{
}

void *PlaybackThread::threadMain(void *data)
{
	PlaybackThread *playback = static_cast<PlaybackThread*>(data);

	if(playback)
	{
		size_t count = 0;

		while(!playback->shutdown())
		{
			TRACEF("Nothing to do here...[%zu]\n", count++);
			usleep(1000*1000*1);
		}
	}
	return NULL;
}

void PlaybackThread::signalShutdown()
{
	if(_mutex.lock())
	{
		_shutdown = true;
		_mutex.unlock();
	}
}

bool PlaybackThread::shutdown()
{
	bool ret = false;

	if(_mutex.lock())
	{
		ret = _shutdown;
		_mutex.unlock();
	}

	return ret;
}
