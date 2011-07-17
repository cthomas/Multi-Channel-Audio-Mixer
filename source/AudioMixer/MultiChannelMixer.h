#ifndef _MULTI_CHANNEL_MIXER_H_
#define _MULTI_CHANNEL_MIXER_H_

#include "BasicAudioChannel.h"
#include "AudioMixerInterface.h"

class MultiChannelMixer
	: public AudioMixerInterface, public BasicAudioChannel
{
public:
	MultiChannelMixer();
	virtual ~MultiChannelMixer();

public: //AudioMixerInterface
	virtual void addChannel(AudioChannelInterface *channel);
	virtual void removeChannel(AudioChannelInterface *channel);
	virtual size_t numChannels() const;
};
#endif
