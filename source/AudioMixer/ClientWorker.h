#ifndef _CLIENT_WORKER_H_
#define _CLIENT_WORKER_H_

#include "BasicAudioChannel.h"
#include "BasicThread.h"

class ClientWorker
	: public BasicAudioChannel,
		public BasicThread
{
private:
	ClientWorker();
	AudioMixerInterface *_mixer;

	static void *threadMain(void *data);
public:
	~ClientWorker();
	static ClientWorker *startClientWorker();
};

#endif
