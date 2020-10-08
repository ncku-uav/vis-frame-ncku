/*
 * test_buffer.cpp
 * Author: thibault
 *
 */


#include <Buffer.hpp>

#ifdef UNIT_TESTING
#include<gtest/gtest.h>

TEST(BufferTest, addToBuffer)
{
	Buffer receive_buffer;

	//test 1
	uint8_t test_string[] = {'t', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 't', 'e', 's', 't'};

	uint16_t size_to_add = sizeof(test_string);
	receive_buffer.add_to_buffer(test_string, size_to_add);

	for (uint8_t i = 0; i < sizeof(test_string); i++)
	{
		EXPECT_EQ(receive_buffer.buffer[i], test_string[i]);
	}

	//test 2
	uint8_t test_string_2[] = {'t', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', '2', 'n', 'd', ' ', 't', 'e', 's', 't'};

	uint16_t size_to_add_2 = sizeof(test_string_2);
	receive_buffer.add_to_buffer(test_string_2, size_to_add_2);

	for (uint8_t i = 0; i < sizeof(test_string_2); i++)
	{
		EXPECT_EQ(receive_buffer.buffer[i+sizeof(test_string)], test_string_2[i]);
	}

}

TEST(BufferTest, getPointerStartAndEnd)
{
	Buffer receive_buffer;

	uint8_t test_string[] = {'t', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 't', 'e', 's', 't'};

	uint16_t size_to_add = sizeof(test_string);
	receive_buffer.add_to_buffer(test_string, size_to_add);

	EXPECT_EQ(receive_buffer.get_start_position(), 0);
	EXPECT_EQ(receive_buffer.get_end_position(), 14);
}

TEST(BufferTest, deleteFromBuffer)
{
	Buffer receive_buffer;

	uint8_t test_string[] = {'t', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 't', 'e', 's', 't'};

	uint16_t size_to_add = sizeof(test_string);
	receive_buffer.add_to_buffer(test_string, size_to_add);

	for (uint8_t i = 0; i < sizeof(test_string); i++)
	{
		EXPECT_EQ(receive_buffer.buffer[i], test_string[i]);
	}

	EXPECT_EQ(receive_buffer.get_buffer_size(), 14);

	receive_buffer.delete_from_buffer(4);

	for (uint16_t i = receive_buffer.get_start_position(); i < receive_buffer.get_end_position(); i++)
	{
		EXPECT_EQ(receive_buffer.buffer[i], test_string[i]);
	}

	EXPECT_EQ(receive_buffer.get_buffer_size(), 10);
}

TEST(BufferTest, clearBuffer)
{
	Buffer receive_buffer;

	uint8_t test_string[] = {'t', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 't', 'e', 's', 't'};

	uint16_t size_to_add = sizeof(test_string);
	receive_buffer.add_to_buffer(test_string, size_to_add);

	receive_buffer.clear_buffer();

	EXPECT_EQ(receive_buffer.get_buffer_size(), 0);
	EXPECT_EQ(receive_buffer.get_start_position(), 0);
	EXPECT_EQ(receive_buffer.get_end_position(), 0);
}


#endif /* UNIT_TESTING */



