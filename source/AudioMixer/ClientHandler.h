#ifndef _CLIENT_HANDLER_H_
#define _CLIENT_HANDLER_H_

#include "BasicThread.h"

class AudioMixerInterface;

class ClientHandler
	: public BasicThread
{
private:
	ClientHandler();
	AudioMixerInterface *_mixer;

	static void *threadMain(void *data);
public:
	~ClientHandler();
	static ClientHandler *startClientHandler(AudioMixerInterface *mixer);
};

#endif
