
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

#include "DebugMessage.h"

#include "AlsaAudioPlayback.h"

unsigned long AlsaPlayback::_volume = 25;
unsigned int AlsaPlayback::_sample_rate = 22050;
//unsigned int AlsaPlayback::_sample_rate = 44100;
static snd_pcm_t *alsa_handle = NULL;
static snd_pcm_uframes_t frames = {0};

AlsaPlayback::AlsaPlayback()
{
	_num_samples_played = 0;
	_recording = false;
	alsa_handle = NULL;

	AlsaPlayback::setupHandle();
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

void AlsaPlayback::playAudio(const std::vector<AudioSample_t> & samples)
{
	if(samples.size() > 0)
	{
		playAudio(&samples[0], samples.size());
	}
}

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
		TRACE("Unable to start recording...path not set\n");
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

