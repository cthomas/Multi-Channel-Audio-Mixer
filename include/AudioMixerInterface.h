#ifndef _AUDIO_MIXER_INTERFACE_H_
#define _AUDIO_MIXER_INTERFACE_H_

#include <string.h>

class AudioChannelInterface;

/**
 * @class AudioMixerInterface
 * @brief Abstract class that defines the interfaces required for an audio mixer
 */
class AudioMixerInterface
{
public:
	/** @brief Pure virtual interface that triggers mixing of all AudioMixerInterface channels */
	virtual void mixDown() = 0;
	/** @brief Pure virtual interface that allows adding of an AudioChannelInterface to the AudioMixerInterface */
	virtual void addChannel(AudioChannelInterface *channel) = 0;
	/** @brief Pure virtual interface that allows removing an AudioChannelInterface from the AudioMixerInterface */
	virtual void removeChannel(AudioChannelInterface *channel) = 0;
	/** @brief Pure virtual interface that will drop all AudioChannelInterface associated with the AudioMixerInterface */
	virtual void dropChannels() = 0;
	/** @brief Pure virtual interface that returns the number of AudioChannelInterface associated with the AudioMixerInterface */
	virtual size_t numChannels() = 0;
	/** @brief Pure virtual interface that allows for blocking on data availability from the AudioMixerInterface */
	virtual void waitData() = 0;
};

#endif
