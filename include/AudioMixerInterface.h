#ifndef _AUDIO_MIXER_INTERFACE_H_
#define _AUDIO_MIXER_INTERFACE_H_

#include <string.h>

class AudioChannelInterface;

class AudioMixerInterface
{
public:
	virtual void mixDown() = 0;
	virtual void addChannel(AudioChannelInterface *channel) = 0;
	virtual void removeChannel(AudioChannelInterface *channel) = 0;
	virtual size_t numChannels() = 0;
	virtual void waitData() = 0;
};

#endif
