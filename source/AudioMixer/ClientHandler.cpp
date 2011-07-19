
#include <unistd.h>

#include "AudioMixerInterface.h"
#include "ClientWorker.h"
#include "DebugMessage.h"
#include "PracticalSocket.h"

#include "ClientHandler.h"

#define PLAYBACK_FILE "test/data/test_22050_1.wav"
#define VOICEOVER_FILE "test/data/voice_over.wav"

ClientHandler::ClientHandler()
	: BasicThread(1024*128)
{
	_mixer = NULL;
	_listen_sock = NULL;
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

#define INTRO_FILE "test/data/birds.wav"

void *ClientHandler::threadMain(void *data)
{
	ClientHandler *handler = static_cast<ClientHandler*>(data);

	if(handler)
	{
		handler->setThreadIdentifier("ClientHandler");
		handler->_listen_sock = new TCPServerSocket(22000, 5);

		ClientWorker *worker = ClientWorker::startClientWorker();
		if(worker)
		{
			handler->_workers.push_back(worker);
			handler->_mixer->addChannel(worker);
			worker->startPlaybackFile(INTRO_FILE);
		}

		size_t sleep_time_secs = 0;
		while(!handler->shutdown())
		{
			try
			{
				if(handler->_listen_sock)
				{
					TCPSocket *client = handler->_listen_sock->accept();

					if(client)
					{
						printf("New client connected...\n");
						ClientWorker *worker = ClientWorker::startClientWorker();

						if(worker)
						{
							worker->startPlaybackSock(client);
							handler->_workers.push_back(worker);
							if(handler->_mixer)
								handler->_mixer->addChannel(worker);
						}
						else
						{
							delete client;
							client = NULL;
						}
					}
				}
			}
			catch (SocketException &e)
			{
				printf("Caught exception from accept [%s]\n", e.what());
			}

			sleep_time_secs += 1;
			usleep(1000*500);
		}

		if(handler->_listen_sock)
		{
			delete handler->_listen_sock;
			handler->_listen_sock = NULL;
		}
	}

	return NULL;
}

