/*
 * USBDevice.cpp
 *
 *  Created on: 08.04.2019
 *      Author: thibault
 */

#include "USBDevice.hpp"
#include <thread>
#include <iostream>

#include <stropts.h>

using namespace std;

USB_Device::USB_Device() {
	// TODO Auto-generated constructor stub

}

USB_Device::~USB_Device() {
	// TODO Auto-generated destructor stub
}

void USB_Device::dev_parser()
{
	//while(1){
		if (b_write == false)
		{
			n = read(dev, &buf_dev, 1);
			if (n > 0) {
//				switch(*buf_dev)
//				{
//				case 0x44:
//					cout << "cool";
//					break;
//
//				default:
//					cout << *buf_dev;
//					break;
//				}
				device_buffer.add_to_buffer(buf_dev,1);
			}
		}
		else if (b_write == true)
		{
			//ioctl(dev,TIOCMBIS,TIOCM_DTR);
			n = write(dev, &write_buffer, write_buffer_size);
			if (n > 0) {
				b_write = false;
				write_buffer_size = 0;
			}
			//ioctl(dev,TIOCMBIC,TIOCM_DTR);
		}
	//}
}

void USB_Device::open_port(char port[], uint16_t baudrate)
{
	/* open serial port */
// 	dev = open(port, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
//	dev = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
//	dev = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
	dev = open(port, O_RDWR | O_NOCTTY);

	printf ("{\"debug\":{\"STATE\":\"opened device with state: %d\"}}\n", dev);
	/* get current serial port settings */
	tcgetattr(dev, &toptions_dev);
	/* set 9600 baud both ways */
	cfsetispeed(&toptions_dev, baudrate);
	cfsetospeed(&toptions_dev, baudrate);

//	toptions_dev.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
//                    | INLCR | IGNCR | ICRNL | IXON);
//	toptions_dev.c_oflag &= ~OPOST;
//	toptions_dev.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
//
//	/* 8 bits, no parity, no stop bits */
//	toptions_dev.c_cflag &= ~PARENB;
//	toptions_dev.c_cflag &= ~CSTOPB;
//	toptions_dev.c_cflag &= ~CSIZE;
//	toptions_dev.c_cflag |= CS8;
//	/* Canonical mode */
//	toptions_dev.c_lflag |= ICANON;

	cfmakeraw(&toptions_dev);

	//toptions_dev.c_lflag &= ~ICANON;
	//toptions_dev.c_cflag |= VEOF;
	//toptions_dev.c_oflag |= OPOST;
	toptions_dev.c_cc[VMIN] = 0;  //sets timeout so that the writer buffer of the serial chip flushed immediately
	toptions_dev.c_cc[VTIME] = 0; //timeout of the ftdi chip is 16ms

	/* commit the serial port settings */
	tcsetattr(dev, TCSANOW, &toptions_dev);
	//fflush(stdout);
}

void USB_Device::create_dev_parser_thread()
{
	//pthread_t thread_id_dev;
	//pthread_create(&thread_id_dev, NULL, dev_parser, NULL);


	std::thread output_thread(&USB_Device::dev_parser,this);
	if(output_thread.joinable())
	{
		output_thread.detach();
	}
}

