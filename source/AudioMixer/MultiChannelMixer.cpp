#include <algorithm>

#include "MultiChannelMixer.h"

bool ExternalLockingAudioChannel::externalLock()
{
	return _mutex.lock();
}

bool ExternalLockingAudioChannel::externalUnlock()
{
	return _mutex.unlock();
}

MultiChannelMixer::MultiChannelMixer()
{

}

MultiChannelMixer::~MultiChannelMixer()
{

}

void MultiChannelMixer::mixDown()
{
	if(_mixer_mutex.lock())
	{
		if(externalLock())
		{
			std::vector<AudioChannelInterface*>::iterator itt = _channels.begin();
			std::vector<AudioSample_t> mixed_samples;

			for(; itt != _channels.end(); ++itt)
			{
				if(*itt)
				{
					std::vector<AudioSample_t> channel_samples = (*itt)->pop_all();

					for(size_t i = 0; i < channel_samples.size(); i++)
					{
						if(i < mixed_samples.size())
						{
							mixed_samples[i] = mixed_samples[i] + channel_samples[i];
						}
						else
						{
							push_back_internal(channel_samples[i]);
						}
					}
				}
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

