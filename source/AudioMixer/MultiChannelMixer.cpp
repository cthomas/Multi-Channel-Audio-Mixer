#include <algorithm>

#include "DebugMessage.h"
#include "MultiChannelMixer.h"

bool ExternalLockingAudioChannel::externalLock()
{
	return _channel_mutex.lock();
}

bool ExternalLockingAudioChannel::externalUnlock()
{
	return _channel_mutex.unlock();
}

MultiChannelMixer::MultiChannelMixer()
{
	pthread_cond_init(&_mixer_cond, NULL);
}

MultiChannelMixer::~MultiChannelMixer()
{
	//Drop any channels we have
	std::vector<AudioChannelInterface*>::iterator itt = _channels.begin();
	for(; itt != _channels.end(); ++itt)
	{
		if((*itt))
		{
			(*itt)->setConditionToSignal(NULL);
		}
	}
	_channels.clear();
	pthread_cond_destroy(&_mixer_cond);
}

void MultiChannelMixer::mixDown()
{
	if(_mixer_mutex.lock())
	{
		if(externalLock())
		{
			if(_sample_queue.size() < MINIMUM_MIX_QUEUE_SIZE)
			{
				std::vector<AudioChannelInterface*>::iterator itt = _channels.begin();
				std::vector<AudioSample_t> mixed_samples;

				for(; itt != _channels.end(); ++itt)
				{
					if(*itt)
					{
						std::vector<AudioSample_t> channel_samples = (*itt)->pop(MINIMUM_MIX_QUEUE_SIZE);


						for(size_t i = 0; i < channel_samples.size(); i++)
						{
							if(i < mixed_samples.size())
							{
								mixed_samples[i] = mixed_samples[i] + channel_samples[i];
							}
							else
							{
								mixed_samples.push_back(channel_samples[i]);
							}
						}
					}
				}

				TRACEF("Mixed down [%zu] samples\n", mixed_samples.size());

				push_back_internal(mixed_samples);
			}
			externalUnlock();
		}
		_mixer_mutex.unlock();
	}
}

void MultiChannelMixer::addChannel(AudioChannelInterface *channel)
{
	if(channel)
	{
		if(_mixer_mutex.lock())
		{
			if(std::find(_channels.begin(), _channels.end(), channel) == _channels.end())
			{
				channel->setConditionToSignal(&_mixer_cond);
				_channels.push_back(channel);
			}

			_mixer_mutex.unlock();
		}
	}
}

void MultiChannelMixer::removeChannel(AudioChannelInterface *channel)
{
	if(channel)
	{
		if(_mixer_mutex.lock())
		{
			std::vector<AudioChannelInterface*>::iterator itt = std::find(_channels.begin(), _channels.end(), channel);

			if(itt != _channels.end())
			{
				if((*itt))
					(*itt)->setConditionToSignal(NULL);
				_channels.erase(itt);
			}

			_mixer_mutex.unlock();
		}
	}
}

size_t MultiChannelMixer::numChannels()
{
	size_t num_channels = 0;

	if(_mixer_mutex.lock())
	{
		num_channels = _channels.size();
		_mixer_mutex.unlock();
	}

	return num_channels;
}

void MultiChannelMixer::waitData()
{
	if(externalLock())
	{
		if(_sample_queue.size() == 0)
		{
			struct timespec timeout = {0};

			clock_gettime(CLOCK_REALTIME, &timeout);
			timeout.tv_sec += 3;
			pthread_cond_timedwait(&_mixer_cond, _channel_mutex.getMutex(), &timeout);
		}
		externalUnlock();
	}
}
