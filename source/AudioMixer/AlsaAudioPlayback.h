#ifndef _ALSA_PLAYBACK_H_
#define _ALSA_PLAYBACK_H_

#include <vector>
#include "AudioPlaybackInterface.h"

/**
 * @class AlsaPlayback
 * @brief ALSA compatible class that implements the AudioPlaybackInterface
 */
class AlsaPlayback : public virtual AudioPlaybackInterface
{
private:
	static unsigned long _volume;
	static unsigned int _sample_rate;
	size_t _num_samples_played;
	std::string _recording_path;
	bool _recording;

	void setupHandle();
public:
	AlsaPlayback();
	virtual ~AlsaPlayback();

	static const unsigned int FRAME_PERIOD = 128;

public: //AudioPlaybackInterface
	/** @brief Interface that will play back the audio data in \asamples */
	virtual void playAudio(const std::vector<AudioSample_t> & samples);
	virtual void playAudio(const AudioSample_t *samples, const size_t num_samples);
	virtual void setVolume(const size_t new_volume);
	virtual size_t getVolume() const;
	virtual void setRecordingOutputPath(const std::string & path);
	virtual const std::string getRecordingOutputPath() const;
	virtual void startRecording();
	virtual void stopRecording();
	virtual void pauseRecording();
	virtual bool currentlyRecording() const;
	virtual size_t getNumSamplesPlayed() const;
	virtual void resetNumSamplesPlayed();
};

#endif
