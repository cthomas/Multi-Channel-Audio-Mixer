#ifndef __CHECK_TEMPLATE_H__
#define __CHECK_TEMPLATE_H__

#include <cxxtest/TestSuite.h>

//#include "Template.h"

class check_Template : public CxxTest::TestSuite
{
public:
	check_Template ()
	{
	}

	~check_Template ()
	{
	}

	void setUp()
	{
	}

	void tearDown()
	{
	}

	void test_template()
	{
		TS_ASSERT(true);
	}
};

#endif

