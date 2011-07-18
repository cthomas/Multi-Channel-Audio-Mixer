
#include <unistd.h>

#include "AudioMixerInterface.h"
#include "DebugMessage.h"

#include "ClientHandler.h"

ClientHandler::ClientHandler()
	: BasicThread(1024*128)
{
	_mixer = NULL;
}

ClientHandler::~ClientHandler()
{
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
		while(!handler->shutdown())
		{
			usleep(1000*1000*1);
		}
	}

	return NULL;
}

