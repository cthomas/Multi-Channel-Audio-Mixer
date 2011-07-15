#ifndef _MULTI_CHANNEL_MIXER_H_
#define _MULTI_CHANNEL_MIXER_H_

#include "AudioMixerInterface.h"

class MultiChannelMixer
	: public AudioMixerInterface
{
public:
	MultiChannelMixer();
	virtual ~MultiChannelMixer();

public: //AudioMixerInterface
	virtual void addChannel(AudioChannelInterface *channel);
	virtual void removeChannel(AudioChannelInterface *channel);
	virtual size_t numChannels() const;

public: //AudioChannelInterface
	virtual void push_back(const AudioSample_t & sample);
	virtual void push_back(const std::vector<AudioSample_t> samples);
	virtual void push_back(const AudioSample_t *samples, size_t num_samples);
	virtual const AudioSample_t pop_front();
	virtual const std::vector<AudioSample_t> pop_all();
	virtual size_t size();
};
#endif
