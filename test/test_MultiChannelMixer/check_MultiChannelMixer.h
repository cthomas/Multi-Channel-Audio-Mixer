#ifndef __CHECK_TEMPLATE_H__
#define __CHECK_TEMPLATE_H__

#include <cxxtest/TestSuite.h>

#include "BasicAudioChannel.h"
#include "MultiChannelMixer.h"

class check_MultiChannelMixer : public CxxTest::TestSuite
{
public:
	MultiChannelMixer mixer;
	BasicAudioChannel channel;

	check_MultiChannelMixer ()
	{
	}

	~check_MultiChannelMixer ()
	{
	}

	void setUp()
	{
	}

	void tearDown()
	{
		mixer.clear();
		channel.clear();

		mixer.dropChannels();
	}

	void test_initial_num_channels_zero()
	{
		TS_ASSERT_EQUALS(0u, mixer.numChannels());
	}

	void test_initial_channel_size_zero()
	{
		TS_ASSERT_EQUALS(0u, mixer.size());
	}

	void test_add_channel_increments_num_channels()
	{
		mixer.addChannel(&channel);

		TS_ASSERT_EQUALS(1u, mixer.numChannels());
	}

	void test_add_channel_twice_only_adds_once()
	{
		mixer.addChannel(&channel);
		mixer.addChannel(&channel);

		TS_ASSERT_EQUALS(1u, mixer.numChannels());
	}

	void test_add_channel_remove_channel_decrements_count()
	{
		mixer.addChannel(&channel);
		mixer.removeChannel(&channel);

		TS_ASSERT_EQUALS(0u, mixer.numChannels());
	}

	void test_add_single_channel_with_no_data_mix_down_has_no_data()
	{
		mixer.addChannel(&channel);
		mixer.mixDown();

		TS_ASSERT_EQUALS(0u, mixer.size());
	}

	void test_add_single_channel_with_some_data_mix_down_has_same_num_samples()
	{
		mixer.addChannel(&channel);
		channel.push_back(1);
		channel.push_back(1);
		channel.push_back(1);
		channel.push_back(1);

		mixer.mixDown();

		TS_ASSERT_EQUALS(4u, mixer.pop_all().size());
	}

	void test_add_single_channel_with_some_data_mix_down_has_same_data()
	{
		mixer.addChannel(&channel);
		channel.push_back(1);
		channel.push_back(1);
		channel.push_back(1);
		channel.push_back(1);

		mixer.mixDown();

		std::vector<AudioSample_t> samples = mixer.pop_all();

		for(size_t i = 0; i < samples.size(); i++)
		{
			TS_ASSERT_EQUALS(1, samples[i]);
		}
	}

	void test_add_single_channel_with_some_data_mix_down_enqueues_from_input_channel()
	{
		mixer.addChannel(&channel);
		channel.push_back(1);
		channel.push_back(1);
		channel.push_back(1);
		channel.push_back(1);

		mixer.mixDown();

		TS_ASSERT_EQUALS(0u, channel.size());
	}

	void test_add_two_channels_one_with_data_one_without_mix_down_has_num_samples_from_larger_channel()
	{
		BasicAudioChannel new_channel;
		mixer.addChannel(&new_channel);
		mixer.addChannel(&channel);
		channel.push_back(1);
		channel.push_back(1);
		channel.push_back(1);
		channel.push_back(1);

		mixer.mixDown();

		TS_ASSERT_EQUALS(4u, mixer.pop_all().size());

		mixer.dropChannels();
	}

	void test_add_two_channels_with_some_data_one_without_mix_down_has_same_data_as_the_populated_channel()
	{
		BasicAudioChannel new_channel;
		mixer.addChannel(&new_channel);
		mixer.addChannel(&channel);
		channel.push_back(1);
		channel.push_back(1);
		channel.push_back(1);
		channel.push_back(1);

		mixer.mixDown();

		std::vector<AudioSample_t> samples = mixer.pop_all();

		for(size_t i = 0; i < samples.size(); i++)
		{
			TS_ASSERT_EQUALS(1, samples[i]);
		}

		mixer.dropChannels();
	}

	void test_add_two_channels_with_different_data_mix_down_mixes_the_two_channels_to_output()
	{
		BasicAudioChannel new_channel;
		mixer.addChannel(&new_channel);
		mixer.addChannel(&channel);
		new_channel.push_back(2);
		new_channel.push_back(2);
		new_channel.push_back(2);
		new_channel.push_back(2);
		channel.push_back(1);
		channel.push_back(1);
		channel.push_back(1);
		channel.push_back(1);

		mixer.mixDown();

		std::vector<AudioSample_t> samples = mixer.pop_all();

		for(size_t i = 0; i < samples.size(); i++)
		{
			TS_ASSERT_EQUALS(3, samples[i]);
		}

		mixer.dropChannels();
	}

	void test_add_two_channels_with_varying_data_mix_down_mixes_the_two_channels_to_output()
	{
		BasicAudioChannel new_channel;
		mixer.addChannel(&new_channel);
		mixer.addChannel(&channel);
		new_channel.push_back(1);
		new_channel.push_back(2);
		new_channel.push_back(3);
		new_channel.push_back(4);
		channel.push_back(4);
		channel.push_back(3);
		channel.push_back(2);
		channel.push_back(1);

		mixer.mixDown();

		std::vector<AudioSample_t> samples = mixer.pop_all();

		for(size_t i = 0; i < samples.size(); i++)
		{
			TS_ASSERT_EQUALS(5, samples[i]);
		}

		mixer.dropChannels();
	}

	void test_add_two_channels_with_varying_length_mix_down_mixes_the_two_channels_to_output()
	{
		BasicAudioChannel new_channel;
		mixer.addChannel(&new_channel);
		mixer.addChannel(&channel);
		new_channel.push_back(1);
		new_channel.push_back(2);
		new_channel.push_back(3);
		new_channel.push_back(4);
		new_channel.push_back(4);
		new_channel.push_back(4);
		new_channel.push_back(4);
		new_channel.push_back(4);

		channel.push_back(4);
		channel.push_back(3);
		channel.push_back(2);
		channel.push_back(1);

		mixer.mixDown();

		std::vector<AudioSample_t> samples = mixer.pop_all();

		for(size_t i = 0; i < 4; i++)
		{
			TS_ASSERT_EQUALS(5, samples[i]);
		}

		for(size_t i = 4; i < samples.size(); i++)
		{
			TS_ASSERT_EQUALS(4, samples[i]);
		}

		mixer.dropChannels();
	}

	void test_add_many_channels_all_different_lengths_mix_down_outputs_a_channel_of_the_greatest_length()
	{
		BasicAudioChannel channel1;
		BasicAudioChannel channel2;
		BasicAudioChannel channel3;
		BasicAudioChannel channel4;

		mixer.addChannel(&channel);
		mixer.addChannel(&channel1);
		mixer.addChannel(&channel2);
		mixer.addChannel(&channel3);
		mixer.addChannel(&channel4);

		channel.push_back(4);

		channel1.push_back(4);
		channel1.push_back(3);

		channel2.push_back(4);
		channel2.push_back(3);
		channel2.push_back(1);

		channel3.push_back(4);
		channel3.push_back(3);
		channel3.push_back(2);
		channel3.push_back(1);
		channel3.push_back(4);
		channel3.push_back(3);
		channel3.push_back(2);
		channel3.push_back(1);

		channel4.push_back(4);
		channel4.push_back(3);
		channel4.push_back(2);
		channel4.push_back(1);
		channel4.push_back(4);
		channel4.push_back(3);
		channel4.push_back(2);
		channel4.push_back(1);
		channel4.push_back(4);
		channel4.push_back(3);
		channel4.push_back(2);
		channel4.push_back(1);

		size_t max_size = channel4.size();

		mixer.mixDown();

		std::vector<AudioSample_t> samples = mixer.pop_all();

		TS_ASSERT_EQUALS(samples.size(), max_size);

		mixer.dropChannels();
	}

	void test_add_many_channels_all_different_lengths_mix_down_outputs_a_channel_that_is_mixed_correctly()
	{
		BasicAudioChannel channel1;
		BasicAudioChannel channel2;
		BasicAudioChannel channel3;
		BasicAudioChannel channel4;

		mixer.addChannel(&channel);
		mixer.addChannel(&channel1);
		mixer.addChannel(&channel2);
		mixer.addChannel(&channel3);
		mixer.addChannel(&channel4);

		channel.push_back(4);
		channel1.push_back(4);
		channel2.push_back(4);
		channel3.push_back(4);
		channel4.push_back(4);

		channel1.push_back(3);
		channel2.push_back(3);
		channel3.push_back(3);
		channel4.push_back(3);

		channel2.push_back(1);
		channel3.push_back(2);
		channel4.push_back(2);

		channel3.push_back(1);
		channel4.push_back(1);

		channel3.push_back(4);
		channel4.push_back(4);

		channel3.push_back(3);
		channel4.push_back(3);

		channel3.push_back(2);
		channel4.push_back(2);

		channel3.push_back(1);
		channel4.push_back(1);

		channel4.push_back(4);

		channel4.push_back(3);

		channel4.push_back(2);

		channel4.push_back(1);

		mixer.mixDown();

		std::vector<AudioSample_t> samples = mixer.pop_all();

		TS_ASSERT_EQUALS(20, samples[0]);
		TS_ASSERT_EQUALS(12, samples[1]);
		TS_ASSERT_EQUALS(5, samples[2]);
		TS_ASSERT_EQUALS(2, samples[3]);
		TS_ASSERT_EQUALS(8, samples[4]);
		TS_ASSERT_EQUALS(6, samples[5]);
		TS_ASSERT_EQUALS(4, samples[6]);
		TS_ASSERT_EQUALS(2, samples[7]);
		TS_ASSERT_EQUALS(4, samples[8]);
		TS_ASSERT_EQUALS(3, samples[9]);
		TS_ASSERT_EQUALS(2, samples[10]);
		TS_ASSERT_EQUALS(1, samples[11]);

		mixer.dropChannels();
	}
};

#endif
