#ifndef _AUDIO_CHANNEL_INTERFACE_H_
#define _AUDIO_CHANNEL_INTERFACE_H_

#include "AudioSample.h"

class AudioChannelInterface
{
public:
	void push_back(const AudioSample_t & sample) = 0;
	void push_back(const std::vector<AudioSample_t> samples) = 0;
	AudioSample_t pop_front() = 0;
	const std::vector<AudioSample_t> pop_all() = 0;
	size_t size() = 0;
};

#endif
