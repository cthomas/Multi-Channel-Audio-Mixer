#ifndef _PLAYBACK_INTERFACE_H_
#define _PLAYBACK_INTERFACE_H_

#include "AudioChannel.h"

class PlaybackInterface
{
	typedef enum _PLAYBACK_INTERFACE_STATUS
	{
		IDLE,
		PLAYING
	}PLAYBACK_INTERFACE_STATUS;
public:
	void setPlaybackChannel(AudioChannel *channel) = 0;
	const PLAYBACK_INTERFACE_STATUS getStatus() const = 0;
};
#endif
