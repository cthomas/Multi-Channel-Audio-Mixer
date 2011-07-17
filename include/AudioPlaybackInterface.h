#ifndef _AUDIO_PLAYBACK_INTERFACE_H_
#define _AUDIO_PLAYBACK_INTERFACE_H_

#include <string>
#include "AudioSample.h"

/**
 * @brief Class that defines the interfaces necessary for audio playback functionality
 */
class AudioPlaybackInterface
{
public:
	virtual ~AudioPlaybackInterface() {};

	/** @brief Interface that will play back the audio data in \asamples */
	virtual void playAudio(const std::vector<AudioSample_t> & samples) = 0;
	virtual void playAudio(const AudioSample_t *samples, const size_t num_samples) = 0;
	virtual void setVolume(const size_t new_volume) = 0;
	virtual size_t getVolume() const = 0;
	virtual void setRecordingOutputPath(const std::string & path) = 0;
	virtual const std::string getRecordingOutputPath() const = 0;
	virtual void startRecording() = 0;
	virtual void stopRecording() = 0;
	virtual void pauseRecording() = 0;
	virtual bool currentlyRecording() const = 0;
	virtual size_t getNumSamplesPlayed() const = 0;
	virtual void resetNumSamplesPlayed() = 0;
};

#endif
