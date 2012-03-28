
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

#include "DebugMessage.h"

#include "AlsaAudioPlayback.h"

unsigned long AlsaPlayback::_volume = 25;
unsigned int AlsaPlayback::_sample_rate = 22050;
//unsigned int AlsaPlayback::_sample_rate = 44100;
static snd_pcm_t *alsa_handle = NULL;
static snd_pcm_uframes_t frames = {0};

/**
* @brief AlsaPlayback constructor
*/
AlsaPlayback::AlsaPlayback()
{
	_num_samples_played = 0;
	_recording = false;
	alsa_handle = NULL;

	AlsaPlayback::setupHandle();
}

/**
* @brief AlsaPlayback destructor
*/
AlsaPlayback::~AlsaPlayback()
{
	if(alsa_handle)
	{
		snd_pcm_close(alsa_handle);
		alsa_handle = NULL;
	}

	snd_config_update_free_global();
}

/**
* @brief Setup the ALSA handle to the audio device
*/
void AlsaPlayback::setupHandle()
{
	if(alsa_handle)
	{
		snd_pcm_close(alsa_handle);
		alsa_handle = NULL;
	}

	if(snd_pcm_open(&alsa_handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0)
	{
		TRACE("Unable to open playback device!!\n");
		alsa_handle = NULL;
	}
	else
	{
		int rc = -1;
		snd_pcm_hw_params_t *params = NULL;

		/* Allocate a hardware parameters object. */
		snd_pcm_hw_params_malloc(&params);

		if(NULL != params)
		{
			int dir = 0;

			/* Fill it in with default values. */
			snd_pcm_hw_params_any(alsa_handle, params);

			/* Set the desired hardware parameters. */
			/* Interleaved mode */
			snd_pcm_hw_params_set_access(alsa_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

			/* Signed 16-bit little-endian format */
			snd_pcm_hw_params_set_format(alsa_handle, params, SND_PCM_FORMAT_S16_LE);

			/* One channel (mono) */
			snd_pcm_hw_params_set_channels(alsa_handle, params, 1);

			/* set sampling rate */
			snd_pcm_hw_params_set_rate_near(alsa_handle, params, &AlsaPlayback::_sample_rate, &dir);

			/* Set period size to 128 frames (samples) */
			frames = AlsaPlayback::FRAME_PERIOD;
			snd_pcm_hw_params_set_period_size_near(alsa_handle, params, &frames, &dir);

			snd_pcm_uframes_t buf_size = MINIMUM_SAMPLE_SET_SIZE;
			snd_pcm_hw_params_set_buffer_size_near(alsa_handle, params, &buf_size);

			/* Write the parameters to the driver */
			rc = snd_pcm_hw_params(alsa_handle, params);

			if(rc >= 0)
			{
				TRACE("AlsaPlayback init completed successfully..\n");
			}
			else
			{
				snd_pcm_close(alsa_handle);
				alsa_handle = NULL;
				TRACE("AlsaPlayback init failed\n");
			}

			snd_pcm_hw_params_free(params);
			params = NULL;
		}
		else
		{
			TRACE("playAudio - snd_pcm_hw_params_alloc() failed\n");
		}
	}
}

/**
* @brief Playback AudioSample_t's in samples
*
* @param samples An std::vector of AudioSample_t to be played back
*/
void AlsaPlayback::playAudio(const std::vector<AudioSample_t> & samples)
{
	if(samples.size() > 0)
	{
		playAudio(&samples[0], samples.size());
	}
}

/**
* @brief Playback AudioSample_t's pointed to by samples
*
* @param samples A pointer to AudioSample_t's to be played
* @param num_samples The number of samples pointed to by samples
*/
void AlsaPlayback::playAudio(const AudioSample_t *samples, const size_t num_samples)
{
	if(samples && (num_samples > 0) && alsa_handle)
	{
		bool error_cond = false;
		size_t frames_sent = 0;
		size_t pad_out = num_samples % AlsaPlayback::FRAME_PERIOD;
		size_t frames_to_send = num_samples + pad_out;

		AudioSample_t *send_samples = new AudioSample_t[frames_to_send];

		if(send_samples)
		{
			int rc = 0;

			memset(send_samples, 0, sizeof(AudioSample_t)*frames_to_send);
			memcpy(send_samples, samples, sizeof(AudioSample_t)*num_samples);

			for(frames_sent = 0, error_cond = false; (frames_sent < frames_to_send) && !error_cond; frames_sent += (size_t)frames)
			{
				if(0 != AlsaPlayback::_volume)
				{
					rc = snd_pcm_writei(alsa_handle, &(send_samples[frames_sent]), frames);

					if(-EPIPE == rc)
					{
						TRACE("playAudio() - underrun occurred in snd_pcm_writei()\n");
						snd_pcm_prepare(alsa_handle);
					}
					else if(-EBADFD == rc)
					{
						TRACE("playAudio() - PCM is not in the right state\n");
						AlsaPlayback::setupHandle();
					}
					else if (-ESTRPIPE == rc)
					{
						TRACE("playAudio() - stream is suspended, attempting recovery\n");
					}
					else if(rc < 0)
					{
						TRACEF("playAudio() - error occurred in snd_pcm_writei() [%d]\n", rc);
						error_cond = true;
					}
					else if(rc != (int)frames)
					{
						TRACE("playAudio() - short write occurred in snd_pcm_writei()\n");
					}
					else
					{
						_num_samples_played += rc;
					}
				}
			}

//			rc = snd_pcm_drain(alsa_handle);

			delete[] send_samples;
			send_samples = NULL;
		}
	}
}

/**
* @brief Set the playback volume
* @note Recording/Mirroring currently unimplemented
* @param new_volume The value to set for the playback volume
*/
void AlsaPlayback::setVolume(const size_t new_volume)
{

}

/**
* @brief Get the current playback volume
* @note Recording/Mirroring currently unimplemented
* @return The current playback volume
*/
size_t AlsaPlayback::getVolume() const
{
	return 0;
}

/**
* @brief Set the path to a file to mirror playback output to
* @note Recording/Mirroring currently unimplemented
* @param path The path to a filesystem location to save the playback output
*/
void AlsaPlayback::setRecordingOutputPath(const std::string & path)
{
	_recording_path = path;
}

/**
* @brief Retrieve the path to the recording file
* @note Recording/Mirroring currently unimplemented
* @return An std::string containing the path to the current playback mirror file
*/
const std::string AlsaPlayback::getRecordingOutputPath() const
{
	return _recording_path;
}

/**
* @brief Begin mirroring/recording playback
* @note Recording/Mirroring currently unimplemented
*/
void AlsaPlayback::startRecording()
{
	if(!_recording_path.empty())
		_recording = true;
	else
		TRACE("Unable to start recording...path not set\n");
}

/**
* @brief Stop mirroring/recording playback
* @note Recording/Mirroring currently unimplemented
*/
void AlsaPlayback::stopRecording()
{
	_recording = false;
}

/**
* @brief Pause mirroring/recording playback
* @note Recording/Mirroring currently unimplemented
*/
void AlsaPlayback::pauseRecording()
{
	_recording = false;
}

/**
* @brief Get whether mirroring/recording is currently enabled
* @note Recording/Mirroring currently unimplemented
* @return true if recording, false otherwise
*/
bool AlsaPlayback::currentlyRecording() const
{
	return _recording;
}

/**
* @brief Retrieve the number of samples played since last call to resetNumSamplesPlayed
*
* @return The number of samples played
*/
size_t AlsaPlayback::getNumSamplesPlayed() const
{
	return _num_samples_played;
}

/**
* @brief Reset the counter keeping track of the number of samples played
*/
void AlsaPlayback::resetNumSamplesPlayed()
{
	_num_samples_played = 0;
}

