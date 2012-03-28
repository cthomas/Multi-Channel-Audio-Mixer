#ifndef _AUDIO_PLAYBACK_INTERFACE_H_
#define _AUDIO_PLAYBACK_INTERFACE_H_

#include <string>
#include "AudioSample.h"

/**
 * @class AudioPlaybackInterface
 * @brief Abstract class that defines the interfaces necessary for audio playback functionality
 */
class AudioPlaybackInterface
{
public:
	virtual ~AudioPlaybackInterface() {};

	/** @brief Pure virtual interface that will play back the audio data contained in the samples vector */
	virtual void playAudio(const std::vector<AudioSample_t> & samples) = 0;
	/** @brief Pure virtual interface that will play back the audio data contained in the samples array up to num_samples */
	virtual void playAudio(const AudioSample_t *samples, const size_t num_samples) = 0;
	/** @brief Pure virtual interface that will set the volume to new_volume */
	virtual void setVolume(const size_t new_volume) = 0;
	/** @brief Pure virtual interface that will return the current volume */
	virtual size_t getVolume() const = 0;
	/** @brief Pure virtual interface that sets the output path to path for recordings */
	virtual void setRecordingOutputPath(const std::string & path) = 0;
	/** @brief Pure virtual interface that gets the output path to path for recordings */
	virtual const std::string getRecordingOutputPath() const = 0;
	/** @brief Pure virtual interface that starts recording */
	virtual void startRecording() = 0;
	/** @brief Pure virtual interface that stops recording */
	virtual void stopRecording() = 0;
	/** @brief Pure virtual interface that pauses recording */
	virtual void pauseRecording() = 0;
	/** @brief Pure virtual interface that returns whether recording is currently taking place or not */
	virtual bool currentlyRecording() const = 0;
	/** @brief Pure virtual interface that gets the number of samples played since last reset */
	virtual size_t getNumSamplesPlayed() const = 0;
	/** @brief Pure virtual interface that resets the counter of samples played since last reset */
	virtual void resetNumSamplesPlayed() = 0;
};

#endif
