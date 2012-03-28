#ifndef _MULTI_CHANNEL_MIXER_H_
#define _MULTI_CHANNEL_MIXER_H_

#include <vector>

#include "AudioMixerInterface.h"
#include "BasicAudioChannel.h"
#include "Mutex.h"

class ExternalLockingAudioChannel
	: public BasicAudioChannel
{
public:
	bool externalLock();
	bool externalUnlock();
};

class MultiChannelMixer
	: public AudioMixerInterface, public ExternalLockingAudioChannel
{
private:
	std::vector<AudioChannelInterface*> _channels;
	Mutex _mixer_mutex;
	pthread_cond_t _mixer_cond;

	void clearChannelVector();
public:
	MultiChannelMixer();
	virtual ~MultiChannelMixer();

public: //AudioMixerInterface
	virtual void mixDown();
	virtual void addChannel(AudioChannelInterface *channel);
	virtual void removeChannel(AudioChannelInterface *channel);
	virtual void dropChannels();
	virtual size_t numChannels();
	virtual void waitData();
};
#endif
