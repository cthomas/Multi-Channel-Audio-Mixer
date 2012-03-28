#ifndef _CLIENT_WORKER_H_
#define _CLIENT_WORKER_H_

#include <string>
#include "BasicAudioChannel.h"
#include "BasicThread.h"

class TCPSocket;

/**
 * @class ClientWorker
 * @brief Class that handles a client connection or file and presents audio data as a BasicAudioChannel
 * @note ClientWorker spawns a thread to run asynchronously to its creator's thread
 * @note Destroying a ClientWorker will force its thread to shutdown and join context with the caller's thread
 */
class ClientWorker
	: public BasicAudioChannel,
		public BasicThread
{
private:
	bool _file_playback;
	std::string _fake_file;
	TCPSocket *_sock;
	ClientWorker();
	pthread_cond_t _worker_cond;

	static void *threadMain(void *data);
	void playFile();
	void playSock();
public:
	~ClientWorker();
	static ClientWorker *startClientWorker();

	void startPlaybackFile(const std::string & filepath);
	void startPlaybackSock(TCPSocket *sock);
};

#endif
