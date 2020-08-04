/*
 * test_main.cpp
 *
 *  Created on: Apr 12, 2019
 *      Author: thibault
 */

#ifdef UNIT_TESTING
#include <gtest/gtest.h>
int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
#endif /* UNIT_TESTING */



