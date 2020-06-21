#ifndef UNIT_TESTING

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <time.h>
#include <pthread.h>
#include <iostream>


#include<telemetry_decoder.hpp>
//#include <telemetry_encoder.hpp>

#include "USBDevice.hpp"
#include "simpleUSBDevice.hpp"
#include "MessageHandler.hpp"

#define DEBUG 0

#define FRSKY
#define ORRX
#define THREEDR

int dev1, dev2, dev3, dev4, n, i;
char buf_dev1[1024] = "temp text1";
char buf_dev2[1024] = "temp text2";
char buf_dev3[1024] = "temp text3";
char buf_dev4[1];
char rec_buf_dev1[1];
struct termios toptions_dev1;
struct termios toptions_dev2;
struct termios toptions_dev3;
struct termios toptions_dev4;
size_t bytes_read;

int main(int argc, char *argv[])
{

	//setbuf(stdout, NULL);
	//setvbuf(stdout, NULL, _IONBF, 0);


	usleep(1000000);

#ifdef FRSKY
	simple_USB_Device frsky_dev;
	frsky_dev.open_port("/dev/ttyUSB_frsky_receiver", B115200);
	frsky_dev.create_dev_parser_thread();
#endif
#ifdef ORRX
	simple_USB_Device orx_dev;
	orx_dev.open_port("/dev/ttyUSB_orRX_receiver", B57600);
	orx_dev.create_dev_parser_thread();
#endif
#ifdef THREEDR
	simple_USB_Device threedr_dev;
	threedr_dev.open_port("/dev/ttyUSB_3DR_receiver", B57600);
	threedr_dev.create_dev_parser_thread();
#endif


//	while(1){}

	MessageHandler messageHandler;
	messageHandler.handleBuffer();

	//TelemetryDecoder* test = new TelemetryDecoder();

	//USB_Device* new_dev = new USB_Device();

//	USB_Device new_dev;
//	new_dev.open_port("/dev/ttyUSB0", B115200);
//	//new_dev->dev_parser();
//
//	new_dev.create_dev_parser_thread();
//
//	while(1)
//	{
//		uint16_t end_position = new_dev.device_buffer.get_end_position();
//		uint16_t start_position = new_dev.device_buffer.get_start_position();
//		if(end_position-start_position == 1)
//		{
//			memcpy(&new_dev.write_buffer, &new_dev.device_buffer.buffer[start_position], 1);
//			new_dev.write_buffer_size = 1;
//			new_dev.b_write=true;
////			for (uint16_t i = 0; i<100; i++)
////			{
////				//std::cout << new_dev.device_buffer.buffer[start_position+i];
////
////			}
//			new_dev.device_buffer.p_buffer_start += 1;
//		}
//		//std::cout << "\n";
//	}



}

#endif /*UNIT_TESTING*/
