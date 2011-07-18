#include <unistd.h>
#include "AudioMixerInterface.h"
#include "DebugMessage.h"

#include "ClientWorker.h"

ClientWorker::ClientWorker()
	: BasicThread(1024*128)
{}

ClientWorker::~ClientWorker()
{

}

void *ClientWorker::threadMain(void *data)
{
	ClientWorker *worker = static_cast<ClientWorker*>(data);

	if(worker)
	{
		while(!worker->shutdown())
		{
			usleep(1000*1000*1);
		}
	}
	return NULL;
}

ClientWorker *ClientWorker::startClientWorker()
{
	ClientWorker *worker = new ClientWorker();

	if(worker && !worker->start(ClientWorker::threadMain, worker))
	{
		delete worker;
		worker = NULL;
	}

	return worker;
}
