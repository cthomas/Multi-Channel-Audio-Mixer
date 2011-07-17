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
	}
	//TODO Moar tests for multiple, differing channels
};

#endif

