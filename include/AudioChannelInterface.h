#ifndef _AUDIO_CHANNEL_INTERFACE_H_
#define _AUDIO_CHANNEL_INTERFACE_H_

#include <pthread.h>
#include <string.h>
#include <vector>

#include "AudioSample.h"

class AudioChannelInterface
{
public:
	virtual void push_back(const AudioSample_t & sample) = 0;
	virtual void push_back(const std::vector<AudioSample_t> samples) = 0;
	virtual void push_back(const AudioSample_t *samples, size_t num_samples) = 0;
	virtual const AudioSample_t pop_front() = 0;
	virtual const std::vector<AudioSample_t> pop_all() = 0;
	virtual const std::vector<AudioSample_t> pop(size_t n) = 0;
	virtual size_t size() = 0;
	virtual void clear() = 0;
	virtual void setConditionToSignal(pthread_cond_t *cond) = 0;
};

#endif
