
#include <unistd.h>

#include "AlsaAudioPlayback.h"
#include "DebugMessage.h"

#include "PlaybackThread.h"

PlaybackThread::PlaybackThread()
{
	_playback_status = PlaybackInterface::IDLE;
	_playback_channel = NULL;
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
		while(!playback->shutdown())
		{
			AudioPlaybackInterface *playback_interface = new AlsaPlayback();
			if(playback_interface && playback->_playback_channel)
			{
				if(playback->_playback_channel->size() > 0)
				{
					std::vector<AudioSample_t> samples = playback->_playback_channel->pop_all();

					if(samples.size())
					{
						TRACE("Playing audio from channel...\n");
//						playback_interface->playAudio(samples, samples.size());
					}
				}

				TRACEF("Num Audio Samples Played [%zu]\n", playback_interface->getNumSamplesPlayed());
			}

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

void PlaybackThread::setPlaybackChannel(AudioChannelInterface *channel)
{
	if(_mutex.lock())
	{
		_playback_channel = channel;
		_mutex.unlock();
	}
}

const PlaybackInterface::PLAYBACK_INTERFACE_STATUS PlaybackThread::getStatus()
{
	PlaybackInterface::PLAYBACK_INTERFACE_STATUS stat = PlaybackInterface::IDLE;

	if(_mutex.lock())
	{
		stat = _playback_status;
		_mutex.unlock();
	}

	return stat;
}
