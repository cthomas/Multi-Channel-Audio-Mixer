
#include <unistd.h>

#include "BasicAudioChannel.h"
#include "DebugMessage.h"
#include "MultiChannelMixer.h"
#include "PlaybackThread.h"

int main(void)
{
	PlaybackThread *playback = PlaybackThread::startPlaybackThread();

	if(playback)
	{
		size_t i = 0;

		while(i++ < 5)
		{
			BasicAudioChannel channel;
			AudioSample_t sample = 0xBEEF;

			for(size_t j = 0; j < 4096; j++)
			{
				channel.push_back(sample);
			}

			playback->addChannel(&channel);

			TRACEF("Main thread sleeping...[%zu]\n", i);
			usleep(1000*1000*5);

			playback->removeChannel(&channel);
		}

		TRACE("Shutting down playback thread...\n");
		delete playback;
		playback = NULL;
	}

	return 0;
}
