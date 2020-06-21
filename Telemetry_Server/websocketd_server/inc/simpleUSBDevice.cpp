/*
 * simpleUSBDevice.cpp
 *
 *  Created on: 08.04.2019
 *      Author: thibault
 */

#include "simpleUSBDevice.hpp"
#include <thread>
#include <iostream>

#include <stropts.h>

using namespace std;

simple_USB_Device::simple_USB_Device() {
	// TODO Auto-generated constructor stub

}

simple_USB_Device::~simple_USB_Device() {
	// TODO Auto-generated destructor stub
}

void simple_USB_Device::dev_parser()
{
	while(1){
	    n = read(dev, buf_dev, 200);
	    /* insert terminating zero in the string */
	    buf_dev[n] = 0;
	    //  printf("%i bytes read, buf_dev1fer contains: %s\n", n, buf_dev1);
	    if(n>0)
	    	printf("%s",buf_dev);
	}
}

void simple_USB_Device::open_port(char port[], unsigned int baudrate)
{
	  /* open serial port */
	  dev = open(port, O_RDWR | O_NOCTTY);
	  /* get current serial port settings */
	  tcgetattr(dev, &toptions_dev);
	  /* set 9600 baud both ways */
	  cfsetispeed(&toptions_dev, baudrate);
	  cfsetospeed(&toptions_dev, baudrate);
	  /* 8 bits, no parity, no stop bits */
	  toptions_dev.c_cflag &= ~PARENB;
	  toptions_dev.c_cflag &= ~CSTOPB;
	  toptions_dev.c_cflag &= ~CSIZE;
	  toptions_dev.c_cflag |= CS8;
	  /* Canonical mode */
	  toptions_dev.c_lflag |= ICANON;
	  /* commit the serial port settings */

	//  cfmakeraw(&toptions_dev1);

	  tcsetattr(dev, TCSANOW, &toptions_dev);
}

void simple_USB_Device::create_dev_parser_thread()
{
	//pthread_t thread_id_dev;
	//pthread_create(&thread_id_dev, NULL, dev_parser, NULL);

	if(dev>0)
	{
		printf("{\"debug\":{\"STATE\":\"opened device with state: %d\"}}\n", dev);
		std::thread output_thread(&simple_USB_Device::dev_parser,this);
//		pthread_create(&(this->thread_id_dev), NULL, &(this->dev_parser), NULL);

		if(output_thread.joinable())
		{
			output_thread.detach();
		}
	}
	else
		printf("{\"debug\":{\"STATE\":\"failed opening device with state: %d\"}}\n", dev);
}

