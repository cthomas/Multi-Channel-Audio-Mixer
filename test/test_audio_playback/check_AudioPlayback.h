#ifndef __CHECK_AUDIO_PLAYBACK_H__
#define __CHECK_AUDIO_PLAYBACK_H__

#include <cxxtest/TestSuite.h>
#include <glib/gstdio.h>
#include <glib.h>
#include <sys/stat.h>

#include "AlsaAudioPlayback.h"

static const std::string recording_path = "./test.wav";

class check_AudioPlayback : public CxxTest::TestSuite
{
public:
	AlsaPlayback *player;
	check_AudioPlayback ()
	{
		player = NULL;
	}

	~check_AudioPlayback ()
	{
	}

	void setUp()
	{
		if(NULL == player)
		{
			player = new AlsaPlayback();
		}
	}

	void tearDown()
	{
		if(player)
		{
			delete player;
			player = NULL;
		}

		remove_file(recording_path);
	}

	size_t playSamples()
	{
		AudioSample_t samples[AlsaPlayback::FRAME_SIZE*2] = {0};
		player->playAudio(&samples[0], AlsaPlayback::FRAME_SIZE*2);

		return AlsaPlayback::FRAME_SIZE*2;
	}

	void remove_file(const std::string & filepath)
	{
		g_unlink(filepath.c_str());
	}

	bool FileExists(const std::string & filepath)
	{
		bool ret = false;

		if(!filepath.empty())
		{
			struct stat buf;
			int status = -1;

			memset(&buf, 0, sizeof(buf));
			status = stat(filepath.c_str(), &buf);

			if(0 == status)
			{
				ret = true;
			}
		}

		return ret;

	}

	void test_default_recording_path_blank()
	{
		TS_ASSERT_EQUALS("", player->getRecordingOutputPath());
	}

	void test_setting_recording_path_is_stored()
	{
		player->setRecordingOutputPath(recording_path);

		TS_ASSERT_EQUALS(recording_path, player->getRecordingOutputPath());
	}

	void test_default_num_samples_played_zero()
	{
		TS_ASSERT_EQUALS(0u, player->getNumSamplesPlayed());
	}

	void test_play_zero_samples_check_num_samples_zero()
	{
		AudioSample_t samples[AlsaPlayback::FRAME_SIZE*2] = {0};
		player->playAudio(&samples[0], 0);

		TS_ASSERT_EQUALS(0u, player->getNumSamplesPlayed());
	}

	void test_play_null_samples_check_num_samples_zero()
	{
		player->playAudio(NULL, AlsaPlayback::FRAME_SIZE*2);

		TS_ASSERT_EQUALS(0u, player->getNumSamplesPlayed());
	}

	void test_play_some_samples_check_num_samples()
	{
		size_t num = playSamples();

		TS_ASSERT_EQUALS(num, player->getNumSamplesPlayed());
	}

	void test_play_some_samples_reset_zeroes_num_samples()
	{
		playSamples();
		player->resetNumSamplesPlayed();

		TS_ASSERT_EQUALS(0u, player->getNumSamplesPlayed());
	}


	void test_play_samples_multiple_times_num_samples_grows()
	{
		size_t num = playSamples();
		num+= playSamples();

		TS_ASSERT_EQUALS(num, player->getNumSamplesPlayed());
	}

	void test_play_samples_file_not_recorded_when_no_path()
	{
		playSamples();

		TS_ASSERT(!FileExists(recording_path));
	}

	void test_default_not_recording()
	{
		TS_ASSERT_EQUALS(false, player->currentlyRecording());
	}

	void test_start_recording_no_path_not_recording()
	{
		player->startRecording();

		TS_ASSERT_EQUALS(false, player->currentlyRecording());
	}

	void test_stop_recording_no_path_not_recording()
	{
		player->stopRecording();

		TS_ASSERT_EQUALS(false, player->currentlyRecording());
	}

	void test_pause_recording_no_path_not_recording()
	{
		player->pauseRecording();

		TS_ASSERT_EQUALS(false, player->currentlyRecording());
	}

	void test_stop_recording_with_path_not_recording()
	{
		player->setRecordingOutputPath(recording_path);
		player->stopRecording();

		TS_ASSERT_EQUALS(false, player->currentlyRecording());
	}

	void test_pause_recording_with_path_not_recording()
	{
		player->setRecordingOutputPath(recording_path);
		player->pauseRecording();

		TS_ASSERT_EQUALS(false, player->currentlyRecording());
	}

	void test_start_recording_with_path_is_recording()
	{
		player->setRecordingOutputPath(recording_path);
		player->startRecording();

		TS_ASSERT_EQUALS(true, player->currentlyRecording());
	}

	void test_start_recording_with_path_file_is_not_written()
	{
		player->setRecordingOutputPath(recording_path);
		player->startRecording();

		TS_ASSERT_EQUALS(false, FileExists(recording_path));
	}

//	void test_start_recording_stop_recording_with_path_file_is_written()
//	{
//		player->setRecordingOutputPath(recording_path);
//		player->startRecording();
//		player->stopRecording();
//
//		TS_ASSERT_EQUALS(true, FileExists(recording_path));
//	}
};

#endif

