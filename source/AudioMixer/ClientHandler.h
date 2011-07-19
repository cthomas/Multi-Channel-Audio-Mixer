#ifndef _CLIENT_HANDLER_H_
#define _CLIENT_HANDLER_H_

#include <vector>
#include "BasicThread.h"

class AudioMixerInterface;
class ClientWorker;
class TCPServerSocket;

class ClientHandler
	: public BasicThread
{
private:
	TCPServerSocket *_listen_sock;
	ClientHandler();
	AudioMixerInterface *_mixer;

	static void *threadMain(void *data);
	std::vector<ClientWorker*> _workers;
public:
	~ClientHandler();
	static ClientHandler *startClientHandler(AudioMixerInterface *mixer);
};

#endif
