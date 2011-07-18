#include <unistd.h>
#include "AudioMixerInterface.h"
#include "DebugMessage.h"

#include "ClientWorker.h"

ClientWorker::ClientWorker()
	: BasicThread(1024*128)
{
	_file_playback = false;
	_fake_file.clear();
}

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
			if(worker->_thread_mutex.lock())
			{
				if(!worker->_fake_file.empty())
				{
					
				}
				worker->_thread_mutex.unlock();
			}
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

void ClientWorker::startPlaybackFile(const std::string & filepath)
{
	if(_thread_mutex.lock())
	{
		_fake_file = filepath;
		//signalFilePlayback();
		_thread_mutex.unlock();
	}
}

