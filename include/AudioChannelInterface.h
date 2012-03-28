#ifndef _AUDIO_CHANNEL_INTERFACE_H_
#define _AUDIO_CHANNEL_INTERFACE_H_

#include <pthread.h>
#include <string.h>
#include <vector>

#include "AudioSample.h"

/**
 * @class AudioChannelInterface
 * @brief Abstract class that describes the interface for an audio channel
 */
class AudioChannelInterface
{
public:
	/** @brief Pure virtual interface used to add an AudioSample_t to the channel */
	virtual void push_back(const AudioSample_t & sample) = 0;
	/** @brief Pure virtual interface used to add a vector of AudioSample_t to the channel */
	virtual void push_back(const std::vector<AudioSample_t> samples) = 0;
	/** @brief Pure virtual interface used to add an array of AudioSample_t to the channel */
	virtual void push_back(const AudioSample_t *samples, size_t num_samples) = 0;
	/** @brief Pure virtual interface used to retrieve an AudioSample_t from the channel */
	virtual const AudioSample_t pop_front() = 0;
	/** @brief Pure virtual interface used to retrieve all AudioSample_t from the channel */
	virtual const std::vector<AudioSample_t> pop_all() = 0;
	/** @brief Pure virtual interface used to retrieve at most n AudioSample_t from the channel */
	virtual const std::vector<AudioSample_t> pop(size_t n) = 0;
	/** @brief Pure virtual interface used to read the current size of the channel */
	virtual size_t size() = 0;
	/** @brief Pure virtual interface used to clear the channel */
	virtual void clear() = 0;
	/** @brief Pure virtual interface used to set the condition signaled by the channel when there is new data*/
	virtual void setConditionToSignal(pthread_cond_t *cond) = 0;
};

#endif
