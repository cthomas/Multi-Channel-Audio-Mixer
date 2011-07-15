
#include "MultiChannelMixer.h"

MultiChannelMixer::MultiChannelMixer()
{

}

MultiChannelMixer::~MultiChannelMixer()
{

}

void MultiChannelMixer::addChannel(AudioChannelInterface *channel)
{

}

void MultiChannelMixer::removeChannel(AudioChannelInterface *channel)
{

}

size_t MultiChannelMixer::numChannels() const
{
	return 0;
}

void MultiChannelMixer::push_back(const AudioSample_t & sample)
{

}

void MultiChannelMixer::push_back(const std::vector<AudioSample_t> samples)
{

}

void MultiChannelMixer::push_back(const AudioSample_t *samples, size_t num_samples)
{

}

const AudioSample_t MultiChannelMixer::pop_front()
{
	AudioSample_t sample = 0;

	return sample;
}

const std::vector<AudioSample_t> MultiChannelMixer::pop_all()
{
	std::vector<AudioSample_t> samples;

	return samples;
}

size_t MultiChannelMixer::size()
{
	return 0;
}

