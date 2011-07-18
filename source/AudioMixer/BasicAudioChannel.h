#ifndef _BASIC_AUDIO_CHANNEL_H_
#define _BASIC_AUDIO_CHANNEL_H_

#include <deque>
#include "AudioChannelInterface.h"
#include "AudioSample.h"
#include "Mutex.h"

class BasicAudioChannel
	: public virtual AudioChannelInterface
{
protected:
	std::deque<AudioSample_t> _sample_queue;
	Mutex _channel_mutex;
	pthread_cond_t *_channel_cond;
	const AudioSample_t pop_front_internal();
	void push_back_internal(const AudioSample_t & sample);
	void push_back_internal(const std::vector<AudioSample_t> samples);
	void signalData();
public:
	BasicAudioChannel();
	virtual ~BasicAudioChannel();

public: //AudioChannelInterface
	virtual void push_back(const AudioSample_t & sample);
	virtual void push_back(const std::vector<AudioSample_t> samples);
	virtual void push_back(const AudioSample_t *samples, size_t num_samples);
	virtual const AudioSample_t pop_front();
	virtual const std::vector<AudioSample_t> pop_all();
	virtual size_t size();
	virtual void clear();
	virtual void setConditionToSignal(pthread_cond_t *cond);
};
#endif
