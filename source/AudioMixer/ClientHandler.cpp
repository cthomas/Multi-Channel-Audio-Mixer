
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
		size_t sleep_time_secs = 0;
		while(!handler->shutdown())
		{
			if(sleep_time_secs == 5)
			{
				ClientWorker *worker = ClientWorker::startClientWorker();

				if(worker)
				{
					if(handler->_mixer)
					{
						handler->_mixer->addChannel(worker);
						worker->startPlaybackFile(PLAYBACK_FILE);
					}

					handler->_workers.push_back(worker);
				}
			}
			else if(sleep_time_secs == 10)
			{
				ClientWorker *worker = ClientWorker::startClientWorker();

				if(worker)
				{
					if(handler->_mixer)
					{
						handler->_mixer->addChannel(worker);
						worker->startPlaybackFile(VOICEOVER_FILE);
					}

					handler->_workers.push_back(worker);
				}
			}
			sleep_time_secs += 1;
			usleep(1000*1000*1);
		}
	}

	return NULL;
}

