
#include "BasicAudioChannel.h"

BasicAudioChannel::~BasicAudioChannel()
{

}

void BasicAudioChannel::push_back_internal(const AudioSample_t & sample)
{
	_sample_queue.push_back(sample);
}

void BasicAudioChannel::push_back_internal(const std::vector<AudioSample_t> samples)
{
	for(size_t i = 0; i < samples.size(); i++)
	{
		push_back_internal(samples[i]);
	}
}

void BasicAudioChannel::push_back(const AudioSample_t & sample)
{
	if(_mutex.lock())
	{
		push_back_internal(sample);
		_mutex.unlock();
	}
}

void BasicAudioChannel::push_back(const std::vector<AudioSample_t> samples)
{
	if(_mutex.lock())
	{
		push_back_internal(samples);
		_mutex.unlock();
	}
}

void BasicAudioChannel::push_back(const AudioSample_t *samples, size_t num_samples)
{
	if((NULL != samples) && (0 < num_samples))
	{
		if(_mutex.lock())
		{
			for(size_t i = 0; i < num_samples; i++)
			{
				push_back_internal(samples[i]);
			}
			_mutex.unlock();
		}
	}
}

const AudioSample_t BasicAudioChannel::pop_front_internal()
{
	AudioSample_t sample = 0;

	if(_sample_queue.size() > 0)
	{
		sample = _sample_queue.front();
		_sample_queue.pop_front();
	}

	return sample;
}

const AudioSample_t BasicAudioChannel::pop_front()
{
	AudioSample_t sample = 0;

	if(_mutex.lock())
	{
		sample = pop_front_internal();
		_mutex.unlock();
	}

	return sample;
}

const std::vector<AudioSample_t> BasicAudioChannel::pop_all()
{
	std::vector<AudioSample_t> samples;

	if(_mutex.lock())
	{
		size_t num_samples = _sample_queue.size();
		for(size_t i = 0; i < num_samples; i++)
		{
			samples.push_back(pop_front_internal());
		}
		_mutex.unlock();
	}

	return samples;
}

size_t BasicAudioChannel::size()
{
	size_t ret = 0;

	if(_mutex.lock())
	{
		ret = _sample_queue.size();
		_mutex.unlock();
	}

	return ret;
}

void BasicAudioChannel::clear()
{
	if(_mutex.lock())
	{
		_sample_queue.clear();

		_mutex.unlock();
	}
}
