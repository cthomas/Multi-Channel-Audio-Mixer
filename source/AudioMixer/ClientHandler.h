#ifndef _CLIENT_HANDLER_H_
#define _CLIENT_HANDLER_H_

#include <vector>
#include "BasicThread.h"

class AudioMixerInterface;
class ClientWorker;
class TCPServerSocket;

/**
 * @class ClientHandler
 * @brief Class that listens for client connections, dispatches a ClientWorker per connection, and ties the client audio channel to the AudioMixerInterface
 * @note This class utilizes BasicThread to spawn a thread and run asynchronously to the thread within which it was instantiated
 * @note Destroying a ClientHandler instance will also force the underlying thread to join to the destroying thread's context
 */
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
