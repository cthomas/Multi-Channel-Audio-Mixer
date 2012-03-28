
#include <unistd.h>

#include "AlsaAudioPlayback.h"
#include "DebugMessage.h"

#include "PlaybackThread.h"

/**
* @brief PlaybackThread constructor
*/
PlaybackThread::PlaybackThread()
	: BasicThread(1024*128)
{}

/**
* @brief PlaybackThread destructor
*/
PlaybackThread::~PlaybackThread()
{}

/**
* @brief Create and start a PlaybackThread
*
* @return A pointer to the created PlaybackThread
*/
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

/**
* @brief Main thread function for a PlaybackThread
* @note This function implements the mixing and playback logic used by the PlaybackThread
* @param data A void pointer to the PlaybackThread instance
*
* @return NULL
*/
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

			std::vector<AudioSample_t> samples = playback->pop(MINIMUM_SAMPLE_SET_SIZE);

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
