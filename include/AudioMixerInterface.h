#ifndef _AUDIO_MIXER_INTERFACE_H_
#define _AUDIO_MIXER_INTERFACE_H_

#include <string.h>

#include "AudioChannelInterface.h"

class AudioMixerInterface
	: public virtual AudioChannelInterface
{
public:
	virtual void addChannel(AudioChannelInterface *channel) = 0;
	virtual void removeChannel(AudioChannelInterface *channel) = 0;
	virtual size_t numChannels() const = 0;
public: //AudioChannelInterface
	virtual void push_back(const AudioSample_t & sample) = 0;
	virtual void push_back(const std::vector<AudioSample_t> samples) = 0;
	virtual void push_back(const AudioSample_t *samples, size_t num_samples) = 0;
	virtual const AudioSample_t pop_front() = 0;
	virtual const std::vector<AudioSample_t> pop_all() = 0;
	virtual size_t size() = 0;

};

#endif
