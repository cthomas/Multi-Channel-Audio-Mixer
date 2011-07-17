#ifndef __CHECK_BASIC_AUDIO_CHANNEL_H__
#define __CHECK_BASIC_AUDIO_CHANNEL_H__

#include <cxxtest/TestSuite.h>

#include "BasicAudioChannel.h"

class check_BasicAudioChannel : public CxxTest::TestSuite
{
public:
	BasicAudioChannel channel;
	check_BasicAudioChannel ()
	{
	}

	~check_BasicAudioChannel ()
	{
	}

	void setUp()
	{
	}

	void tearDown()
	{
		channel.clear();
	}

	void test_initial_size_zero()
	{
		TS_ASSERT_EQUALS(0u, channel.size());
	}

	void test_initial_pop_front_is_zero_sample()
	{
		AudioSample_t sample = 0;
		TS_ASSERT_EQUALS(sample, channel.pop_front());
	}

	void test_initial_pop_all_vector_is_empty()
	{
		TS_ASSERT_EQUALS(0u, channel.pop_all().size());
	}

	void test_add_single_sample_clear_resets_size()
	{
		AudioSample_t sample = 0;

		channel.push_back(sample);

		TS_ASSERT_EQUALS(1u, channel.size());

		channel.clear();

		TS_ASSERT_EQUALS(0u, channel.size());

	}

	void test_add_single_sample_size_is_incremented()
	{
		AudioSample_t sample = 0;

		channel.push_back(sample);

		TS_ASSERT_EQUALS(1u, channel.size());
	}

	void test_add_many_samples_ptr_size_is_incremented()
	{
		AudioSample_t samples[10] = {0};

		channel.push_back(&samples[0], sizeof(samples));

		TS_ASSERT_EQUALS(sizeof(samples), channel.size());
	}

	void test_add_many_samples_size_is_incremented()
	{
		AudioSample_t sample = 0;
		std::vector<AudioSample_t> samples;

		for(size_t i = 0; i <= 10; i++)
		{
			samples.push_back(sample);
		}

		channel.push_back(samples);

		TS_ASSERT_EQUALS(samples.size(), channel.size());
	}

	void test_add_sample_get_same_sample_back()
	{
		AudioSample_t sample = 1;

		channel.push_back(sample);

		TS_ASSERT_EQUALS(sample, channel.pop_front());
	}

	void test_add_sample_get_same_sample_back_decrements_size()
	{
		AudioSample_t sample = 1;

		channel.push_back(sample);

		channel.pop_front();

		TS_ASSERT_EQUALS(0u, channel.size());
	}

	void test_add_sample_pop_all_gets_nonzero_samples_back()
	{
		AudioSample_t sample = 1;

		channel.push_back(sample);

		TS_ASSERT_EQUALS(1u, channel.pop_all().size());
	}

	void test_add_sample_pop_all_gets_correct_samples_back()
	{
		AudioSample_t sample = 1;

		channel.push_back(sample);

		TS_ASSERT_EQUALS(sample, channel.pop_all()[0]);
	}

	void test_add_two_samples_retrieve_in_order()
	{
		AudioSample_t sample1 = 1;
		AudioSample_t sample2 = 2;

		channel.push_back(sample1);
		channel.push_back(sample2);

		TS_ASSERT_EQUALS(sample1, channel.pop_front())
		TS_ASSERT_EQUALS(sample2, channel.pop_front());
	}

	void test_add_two_samples_retrieve_vector_in_order()
	{
		AudioSample_t sample1 = 1;
		AudioSample_t sample2 = 2;

		channel.push_back(sample1);
		channel.push_back(sample2);

		std::vector<AudioSample_t> samples = channel.pop_all();

		TS_ASSERT_EQUALS(sample1, samples[0])
		TS_ASSERT_EQUALS(sample2, samples[1]);
	}
};

#endif

