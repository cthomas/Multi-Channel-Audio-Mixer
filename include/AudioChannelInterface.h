#ifndef _AUDIO_CHANNEL_INTERFACE_H_
#define _AUDIO_CHANNEL_INTERFACE_H_

#include <vector>

#include "AudioSample.h"

class AudioChannelInterface
{
public:
	virtual void push_back(const AudioSample_t & sample) = 0;
	virtual void push_back(const std::vector<AudioSample_t> samples) = 0;
	virtual void push_back(const AudioSample_t *samples, size_t num_samples) = 0;
	virtual AudioSample_t pop_front() = 0;
	virtual const std::vector<AudioSample_t> pop_all() = 0;
	virtual size_t size() = 0;
};

#endif
