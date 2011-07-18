
#include <unistd.h>

#include "AudioMixerInterface.h"
#include "ClientWorker.h"
#include "DebugMessage.h"

#include "ClientHandler.h"

#define PLAYBACK_FILE "test/data/test_22050_1.wav"
#define VOICEOVER_FILE "test/data/voice_over.wav"

ClientHandler::ClientHandler()
	: BasicThread(1024*128)
{
	_mixer = NULL;
}

ClientHandler::~ClientHandler()
{
	std::vector<ClientWorker*>::iterator itt = _workers.begin();

	for(; itt != _workers.end(); ++itt)
	{
		if(*itt)
		{
			if(_mixer)
			{
				_mixer->removeChannel(*itt);
			}

			delete *itt;
			*itt = NULL;
		}
	}

	_workers.clear();
	_mixer = NULL;
}

ClientHandler *ClientHandler::startClientHandler(AudioMixerInterface *mixer)
{
	ClientHandler *handler = new ClientHandler();

	if(handler)
	{
		handler->_mixer = mixer;

		if(!handler->start(ClientHandler::threadMain, handler))
		{
			delete handler;
			handler = NULL;
		}
	}

	return handler;
}

void *ClientHandler::threadMain(void *data)
{
	ClientHandler *handler = static_cast<ClientHandler*>(data);

	if(handler)
	{
		handler->setThreadIdentifier("ClientHandler");

		ClientWorker *worker1 = ClientWorker::startClientWorker();
		ClientWorker *worker2 = ClientWorker::startClientWorker();
		handler->_workers.push_back(worker1);
		handler->_workers.push_back(worker2);
		handler->_mixer->addChannel(worker1);
		handler->_mixer->addChannel(worker2);

		size_t sleep_time_secs = 0;
		while(!handler->shutdown())
		{
			if(sleep_time_secs == 5)
			{
				if(worker1)
				{
					worker1->startPlaybackFile(PLAYBACK_FILE);
				}
			}
			else if(sleep_time_secs == 10)
			{
				if(worker2)
				{
					worker2->startPlaybackFile(VOICEOVER_FILE);
				}
			}
			sleep_time_secs += 1;
			usleep(1000*1000*1);
		}
	}

	return NULL;
}

