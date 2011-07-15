#ifndef _PLAYBACK_INTERFACE_H_
#define _PLAYBACK_INTERFACE_H_

#include "AudioChannelInterface.h"

class PlaybackInterface
{
public:
	typedef enum _PLAYBACK_INTERFACE_STATUS
	{
		IDLE,
		PLAYING
	}PLAYBACK_INTERFACE_STATUS;

	virtual void setPlaybackChannel(AudioChannelInterface *channel) = 0;
	virtual const PLAYBACK_INTERFACE_STATUS getStatus() = 0;
};
#endif
