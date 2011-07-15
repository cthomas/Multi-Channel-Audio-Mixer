#ifndef __CHECK_TEMPLATE_H__
#define __CHECK_TEMPLATE_H__

#include <cxxtest/TestSuite.h>

#include "MultiChannelMixer.h"

class check_MultiChannelMixer : public CxxTest::TestSuite
{
public:
	MultiChannelMixer mixer;
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
	}

	void test_initial_num_channels_zero()
	{
		TS_ASSERT_EQUALS(0u, mixer.numChannels());
	}
};

#endif

