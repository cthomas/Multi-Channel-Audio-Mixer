
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

#include "DebugMessage.h"

#include "AlsaAudioPlayback.h"

unsigned long AlsaPlayback::_volume = 25;
unsigned int AlsaPlayback::_sample_rate = 22050;
static snd_pcm_t *alsa_handle = NULL;

AlsaPlayback::AlsaPlayback()
{
	_num_samples_played = 0;
	_recording = false;
	alsa_handle = NULL;

	if(snd_pcm_open(&alsa_handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0)
	{
		TRACE("Unable to open playback device!!");
		alsa_handle = NULL;
	}
}

AlsaPlayback::~AlsaPlayback()
{
	if(alsa_handle)
	{
		snd_pcm_close(alsa_handle);
		alsa_handle = NULL;
	}

	snd_config_update_free_global();
}

void AlsaPlayback::playAudio(const AudioSample_t *samples, const size_t num_samples)
{
	if(samples && (num_samples > 0) && alsa_handle)
	{
		int rc = -1;
		snd_pcm_hw_params_t *params = NULL;

		/* Allocate a hardware parameters object. */
		snd_pcm_hw_params_malloc(&params);

		if(NULL != params)
		{
			snd_pcm_uframes_t frames = {0};
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

			/* Write the parameters to the driver */
			rc = snd_pcm_hw_params(alsa_handle, params);

			if(rc >= 0)
			{
				bool error_cond = false;
				size_t frames_sent = 0;
				size_t frames_to_send = num_samples;
				for(frames_sent = 0, error_cond = false; (frames_sent < frames_to_send) && !error_cond; frames_sent += (size_t)frames)
				{
					if(0 != AlsaPlayback::_volume)
					{

						rc = snd_pcm_writei(alsa_handle, &(samples[frames_sent]), frames);

						if(-EPIPE == rc)
						{
							TRACE("playAudio() - underrun occurred in snd_pcm_writei()");
							snd_pcm_prepare(alsa_handle);
						}
						else if(rc < 0)
						{
							TRACE("playAudio() - error occurred in snd_pcm_writei()");
							error_cond = true;
						}
						else if(rc != (int)frames)
						{
							TRACE("playAudio() - short write occurred in snd_pcm_writei()");
						}
						else
						{
							_num_samples_played += rc;
						}
					}
				}

				rc = snd_pcm_drain(alsa_handle);
			}
			else
			{
				TRACE("playAudio() - snd_pcm_hw_params() failed");
			}

			snd_pcm_hw_params_free(params);
			params = NULL;
		}
		else
		{
			TRACE("playAudio - snd_pcm_hw_params_alloc() failed");
		}
	}
}

void AlsaPlayback::setVolume(const size_t new_volume)
{

}

size_t AlsaPlayback::getVolume() const
{
	return 0;
}

void AlsaPlayback::setRecordingOutputPath(const std::string & path)
{
	_recording_path = path;
}

const std::string AlsaPlayback::getRecordingOutputPath() const
{
	return _recording_path;
}

void AlsaPlayback::startRecording()
{
	if(!_recording_path.empty())
		_recording = true;
	else
		TRACE("Unable to start recording...path not set");
}

void AlsaPlayback::stopRecording()
{
	_recording = false;
}

void AlsaPlayback::pauseRecording()
{
	_recording = false;
}

bool AlsaPlayback::currentlyRecording() const
{
	return _recording;
}

size_t AlsaPlayback::getNumSamplesPlayed() const
{
	return _num_samples_played;
}

void AlsaPlayback::resetNumSamplesPlayed()
{
	_num_samples_played = 0;
}

