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

	while(1)
	{
		if(!app_signaled)
			usleep(1000*1000*1);
		else
			break;
	}

	if(playback)
	{
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
