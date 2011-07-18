#include "DebugMessage.h"

#include "BasicAudioChannel.h"

BasicAudioChannel::BasicAudioChannel()
{
	_channel_cond = NULL;
}

BasicAudioChannel::~BasicAudioChannel()
{
	_channel_cond = NULL;
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
	if(_channel_mutex.lock())
	{
		push_back_internal(sample);
		signalData();
		_channel_mutex.unlock();
	}
}

void BasicAudioChannel::push_back(const std::vector<AudioSample_t> samples)
{
	if(_channel_mutex.lock())
	{
		push_back_internal(samples);
		signalData();
		_channel_mutex.unlock();
	}
}

void BasicAudioChannel::push_back(const AudioSample_t *samples, size_t num_samples)
{
	if((NULL != samples) && (0 < num_samples))
	{
		if(_channel_mutex.lock())
		{
			for(size_t i = 0; i < num_samples; i++)
			{
				push_back_internal(samples[i]);
			}

			signalData();
			_channel_mutex.unlock();
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

	if(_channel_mutex.lock())
	{
		sample = pop_front_internal();
		_channel_mutex.unlock();
	}

	return sample;
}

const std::vector<AudioSample_t> BasicAudioChannel::pop_all()
{
	std::vector<AudioSample_t> samples;

	if(_channel_mutex.lock())
	{
		size_t num_samples = _sample_queue.size();
		for(size_t i = 0; i < num_samples; i++)
		{
			samples.push_back(pop_front_internal());
		}
		_channel_mutex.unlock();
	}

	return samples;
}

size_t BasicAudioChannel::size()
{
	size_t ret = 0;

	if(_channel_mutex.lock())
	{
		ret = _sample_queue.size();
		_channel_mutex.unlock();
	}

	return ret;
}

void BasicAudioChannel::clear()
{
	if(_channel_mutex.lock())
	{
		_sample_queue.clear();

		_channel_mutex.unlock();
	}
}

void BasicAudioChannel::signalData()
{
	if(_channel_cond && (0 != pthread_cond_signal(_channel_cond)))
		TRACE("Unable to signal condition!\n");
}

void BasicAudioChannel::setConditionToSignal(pthread_cond_t *cond)
{
	if(_channel_mutex.lock())
	{
		_channel_cond = cond;
		_channel_mutex.unlock();
	}
}
