#include <signal.h>
#include <unistd.h>

#include "BasicAudioChannel.h"
#include "ClientHandler.h"
#include "DebugMessage.h"
#include "MultiChannelMixer.h"
#include "PlaybackThread.h"

#define AUDIO_FILE "/home/ct/data/dev/ece554_project/test/data/test_22050_1.wav"

static bool app_signaled = false;

void sig_handler(int num)
{
	TRACEF("Received signal [%d]\n", num);
	app_signaled = true;
}

void setup_signals(void)
{
	for(int i = 0; i <= 15; i++)
	{
		signal(i, sig_handler);
	}
}

int main(void)
{
	setup_signals();

	PlaybackThread *playback = PlaybackThread::startPlaybackThread();
	ClientHandler *client_handler = ClientHandler::startClientHandler(playback);

	if(playback)
	{
		BasicAudioChannel channel;

		playback->addChannel(&channel);
		FILE *pFile = fopen(AUDIO_FILE, "rb");

		if(pFile)
		{
			fseek(pFile, 40, SEEK_SET);

			int32_t data_size = 0;
			fread(&data_size, sizeof(int32_t), 1, pFile);

			AudioSample_t *samples = new AudioSample_t[data_size];

			if(samples)
			{
				fseek(pFile, 44, SEEK_SET);
				fread(samples, data_size, 1, pFile);
			}

			fclose(pFile);
			pFile = NULL;

			size_t samples_to_push = 11264;

			for(size_t i = 0; i < data_size/sizeof(AudioSample_t) && !app_signaled;)
			{
				size_t actual_samples_pushed = (i + samples_to_push < data_size/sizeof(AudioSample_t)) ? samples_to_push : data_size/sizeof(AudioSample_t)-i;
				channel.push_back(&samples[i], actual_samples_pushed);
				i+=samples_to_push;

				usleep(1000*500);
			}

//			channel.push_back(samples, data_size/sizeof(AudioSample_t));
			delete[] samples;
			samples = NULL;
		}

		if(!app_signaled)
			usleep(1000*1000*50);
		TRACE("Shutting down playback thread...\n");
		delete playback;
		playback = NULL;
	}

	if(client_handler)
	{
		delete client_handler;
		client_handler = NULL;
	}

	return 0;
}
