#ifndef _CLIENT_WORKER_H_
#define _CLIENT_WORKER_H_

#include <string>
#include "BasicAudioChannel.h"
#include "BasicThread.h"

class ClientWorker
	: public BasicAudioChannel,
		public BasicThread
{
private:
	bool _file_playback;
	std::string _fake_file;
	ClientWorker();

	static void *threadMain(void *data);
	void signalFilePlayback();
public:
	~ClientWorker();
	static ClientWorker *startClientWorker();

	void startPlaybackFile(const std::string & filepath);
};

#endif
