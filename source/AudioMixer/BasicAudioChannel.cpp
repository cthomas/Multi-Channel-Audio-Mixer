#include "DebugMessage.h"

#include "BasicAudioChannel.h"

/**
* @brief BasicAudioChannel constructor
*/
BasicAudioChannel::BasicAudioChannel()
{
	_channel_cond = NULL;
}

/**
* @brief BasicAudioChannel destructor
*/
BasicAudioChannel::~BasicAudioChannel()
{
	_channel_cond = NULL;
}

/**
* @brief Add a single AudioSample_t to the end of the BasicAudioChannel storage
*
* @param sample An AudioSample_t to add
*/
void BasicAudioChannel::push_back_internal(const AudioSample_t & sample)
{
	_sample_queue.push_back(sample);
}

/**
* @brief Add a set of AudioSample_t to the end of the BasicAudioChannel storage
*
* @param samples An std::vector of AudioSample_t to add
*/
void BasicAudioChannel::push_back_internal(const std::vector<AudioSample_t> samples)
{
	for(size_t i = 0; i < samples.size(); i++)
	{
		push_back_internal(samples[i]);
	}
}

/**
* @brief Add a single AudioSample_t to the end of the BasicAudioChannel storage
*
* @param sample An AudioSample_t to add
*/
void BasicAudioChannel::push_back(const AudioSample_t & sample)
{
	if(_channel_mutex.lock())
	{
		push_back_internal(sample);
		signalData();
		_channel_mutex.unlock();
	}
}

/**
* @brief Add a set of AudioSample_t to the end of the BasicAudioChannel storage
*
* @param samples An std::vector of AudioSample_t to add
*/
void BasicAudioChannel::push_back(const std::vector<AudioSample_t> samples)
{
	if(_channel_mutex.lock())
	{
		push_back_internal(samples);
		signalData();
		_channel_mutex.unlock();
	}
}

/**
* @brief Add a set of AudioSample_t to the end of the BasicAudioChannel storage
*
* @param samples A pointer to AudioSample_t to add
* @param num_samples The number of AudioSample_t pointed to by samples
*/
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

/**
* @brief Remove a single AudioSample_t from the BasicAudioChannel storage
*
* @return The AudioSample_t that was removed from the BasicAudioChannel storage
*/
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

/**
* @brief Remove a single AudioSample_t from the BasicAudioChannel storage
*
* @return The AudioSample_t that was removed from the BasicAudioChannel storage
*/
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

/**
* @brief Remove all of the AudioSample_t from the BasicAudioChannel storage
*
* @return An std::vector of AudioSample_t that was removed from the BasicAudioChannel storage
*/
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

/**
* @brief Remove a specific number of AudioSample_t from the BasicAudioChannel storage
*
* @param n The maximum number of AudioSample_t to remove
*
* @return An std::vector of AudioSample_t of max size n
*/
const std::vector<AudioSample_t> BasicAudioChannel::pop(size_t n)
{
	std::vector<AudioSample_t> samples;

	if(_channel_mutex.lock())
	{
		size_t num_samples = _sample_queue.size() > n ? n : _sample_queue.size();
		for(size_t i = 0; i < num_samples; i++)
		{
			samples.push_back(pop_front_internal());
		}
		_channel_mutex.unlock();
	}

	return samples;
}

/**
* @brief Get the number of AudioSample_t in the BasicAudioChannel storage
*
* @return The number of AudioSample_t in the BasicAudioChannel storage
*/
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

/**
* @brief Blindly remove all AudioSample_t from the BasicAudioChannel storage
*/
void BasicAudioChannel::clear()
{
	if(_channel_mutex.lock())
	{
		_sample_queue.clear();

		_channel_mutex.unlock();
	}
}

/**
* @brief Signal data has been added to the BasicAudioChannel
*/
void BasicAudioChannel::signalData()
{
	if(_channel_cond && (0 != pthread_cond_signal(_channel_cond)))
		TRACE("Unable to signal condition!\n");
}

/**
* @brief Set the pthread_cond_t to signal when data is available
*
* @param cond A pthread_cond_t pointer
*/
void BasicAudioChannel::setConditionToSignal(pthread_cond_t *cond)
{
	if(_channel_mutex.lock())
	{
		_channel_cond = cond;
		_channel_mutex.unlock();
	}
}
