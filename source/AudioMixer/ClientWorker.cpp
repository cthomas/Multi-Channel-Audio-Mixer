#include <unistd.h>
#include "AudioMixerInterface.h"
#include "DebugMessage.h"

#include "ClientWorker.h"

ClientWorker::ClientWorker()
	: BasicThread(1024*128)
{
	_file_playback = false;
	_fake_file.clear();
	pthread_cond_init(&_worker_cond, NULL);
}

ClientWorker::~ClientWorker()
{
	pthread_cond_signal(&_worker_cond);
	pthread_cond_destroy(&_worker_cond);
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
					worker->_file_playback = true;
					worker->_thread_mutex.unlock();
					worker->playFile();
					worker->_thread_mutex.lock();
					worker->_file_playback = false;
				}

				pthread_cond_wait(&worker->_worker_cond, worker->_thread_mutex.getMutex());
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
		pthread_cond_signal(&_worker_cond);
		_thread_mutex.unlock();
	}
}

void ClientWorker::playFile()
{
	_thread_mutex.lock();
	std::string file_to_play = _fake_file;
	_thread_mutex.unlock();

	if(!file_to_play.empty())
	{
		FILE *pFile = fopen(file_to_play.c_str(), "rb");

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

			for(size_t i = 0; i < data_size/sizeof(AudioSample_t) && !shutdown();)
			{
				size_t actual_samples_pushed = (i + samples_to_push < data_size/sizeof(AudioSample_t)) ? samples_to_push : data_size/sizeof(AudioSample_t)-i;
				push_back(&samples[i], actual_samples_pushed);
				i+=samples_to_push;

				usleep(1000*500);
			}

			delete[] samples;
			samples = NULL;
		}
	}
}

