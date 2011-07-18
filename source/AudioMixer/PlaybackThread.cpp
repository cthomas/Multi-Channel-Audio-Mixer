
#include <unistd.h>

#include "AlsaAudioPlayback.h"
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
		AudioPlaybackInterface *playback_interface = new AlsaPlayback();

		while(!playback->shutdown())
		{
			//Pump the mixer
			playback->mixDown();

			std::vector<AudioSample_t> samples = playback->pop_all();

			if(samples.size() > 0)
			{
				TRACE("Playing audio from channel...\n");
				playback_interface->playAudio(samples);
				TRACEF("Num Audio Samples Played [%zu]\n", playback_interface->getNumSamplesPlayed());

			}
		}

		if(playback_interface)
		{
			delete playback_interface;
			playback_interface = NULL;
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
