
#include <unistd.h>

#include "AlsaAudioPlayback.h"
#include "DebugMessage.h"

#include "PlaybackThread.h"

PlaybackThread::PlaybackThread()
	: BasicThread(1024*128)
{}

PlaybackThread::~PlaybackThread()
{}

PlaybackThread *PlaybackThread::startPlaybackThread()
{
	PlaybackThread *playback = new PlaybackThread();

	if(playback && !playback->start(PlaybackThread::threadMain, playback))
	{
		delete playback;
		playback = NULL;
	}

	return playback;
}

void *PlaybackThread::threadMain(void *data)
{
	PlaybackThread *playback = static_cast<PlaybackThread*>(data);

	if(playback)
	{
		playback->setThreadIdentifier("Playback");
		AudioPlaybackInterface *playback_interface = new AlsaPlayback();

		while(!playback->shutdown())
		{
			//Pump the mixer
			playback->mixDown();

			std::vector<AudioSample_t> samples = playback->pop_all();

			if(samples.size() > 0)
			{
				playback_interface->playAudio(samples);
				TRACEF("Num Audio Samples Played [%zu]\n", playback_interface->getNumSamplesPlayed());
			}

			playback->waitData();
		}

		if(playback_interface)
		{
			delete playback_interface;
			playback_interface = NULL;
		}
	}

	return NULL;
}
