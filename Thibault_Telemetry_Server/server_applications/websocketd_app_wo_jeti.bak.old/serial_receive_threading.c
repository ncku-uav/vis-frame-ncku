/* www.chrisheydrick.com

   June 23 2012
   CanonicalArduinoRead write a byte to an Arduino, and then
   receives a serially transmitted string in response.
   The call/response Arduino sketch is here:
   https://gist.github.com/2980344
   Arduino sketch details at www.chrisheydrick.com
*/

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

#define DEBUG 0

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

uint32_t get_interval = (CLOCKS_PER_SEC/1000)*500;

void *dev1_parser(void *vargp)
{
  while(1){
    n = read(dev1, buf_dev1, 1024);
    /* insert terminating zero in the string */
    buf_dev1[n] = 0;
    //  printf("%i bytes read, buf_dev1fer contains: %s\n", n, buf_dev1);
    printf("%s",buf_dev1);
  }
}

void *dev2_parser(void *vargp)
{
  while(1){
    n = read(dev2, buf_dev2, 1024);
    /* insert terminating zero in the string */
    buf_dev2[n] = 0;
    //  printf("%i bytes read, buf_dev1fer contains: %s\n", n, buf_dev1);
    printf("%s",buf_dev2);
  }
}

void *dev3_parser(void *vargp)
{
  while(1){
    n = read(dev3, buf_dev3, 1024);
    /* insert terminating zero in the string */
    buf_dev3[n] = 0;
    //  printf("%i bytes read, buf_dev1fer contains: %s\n", n, buf_dev1);
    printf("%s",buf_dev3);
  }
}

void *dev4_parser(void *vargp)
{
  while(1){
    n = read(dev4, buf_dev4, 1);
    /* insert terminating zero in the string */
//    buf_dev4[n] = 0;
    //  printf("%i bytes read, buf_dev1fer contains: %s\n", n, buf_dev1);
    printf("%s",buf_dev4);
  }
}

int main(int argc, char *argv[])
{

//  setbuf(stdout, NULL);

  /* wait for the Arduino to reboot */
  usleep(1000000);

  /* open serial port */
  dev1 = open("/dev/ttyUSB_frsky_receiver", O_RDWR | O_NOCTTY);
  /* get current serial port settings */
  tcgetattr(dev1, &toptions_dev1);
  /* set 9600 baud both ways */
  cfsetispeed(&toptions_dev1, B115200);
  cfsetospeed(&toptions_dev1, B115200);
  /* 8 bits, no parity, no stop bits */
  toptions_dev1.c_cflag &= ~PARENB;
  toptions_dev1.c_cflag &= ~CSTOPB;
  toptions_dev1.c_cflag &= ~CSIZE;
  toptions_dev1.c_cflag |= CS8;
  /* Canonical mode */
  toptions_dev1.c_lflag |= ICANON;
  /* commit the serial port settings */

//  cfmakeraw(&toptions_dev1);

  tcsetattr(dev1, TCSANOW, &toptions_dev1);

  // open serial port
  dev2 = open("/dev/ttyUSB_orRX_receiver", O_RDWR | O_NOCTTY);
  // get current serial port settings
  tcgetattr(dev2, &toptions_dev2);
  // set 9600 baud both ways
  cfsetispeed(&toptions_dev2, B57600);
  cfsetospeed(&toptions_dev2, B57600);
  // 8 bits, no parity, no stop bits
  toptions_dev2.c_cflag &= ~PARENB;
  toptions_dev2.c_cflag &= ~CSTOPB;
  toptions_dev2.c_cflag &= ~CSIZE;
  toptions_dev2.c_cflag |= CS8;
  // Canonical mode
  toptions_dev2.c_lflag |= ICANON;
  // commit the serial port settings
  tcsetattr(dev2, TCSANOW, &toptions_dev2);

  /* open serial port */
  dev3 = open("/dev/ttyUSB_3DR_receiver", O_RDWR | O_NOCTTY);
  /* get current serial port settings */
  tcgetattr(dev3, &toptions_dev3);
  /* set 9600 baud both ways */
  cfsetispeed(&toptions_dev3, B57600);
  cfsetospeed(&toptions_dev3, B57600);
  /* 8 bits, no parity, no stop bits */
  toptions_dev3.c_cflag &= ~PARENB;
  toptions_dev3.c_cflag &= ~CSTOPB;
  toptions_dev3.c_cflag &= ~CSIZE;
  toptions_dev3.c_cflag |= CS8;
  /* Canonical mode */
  toptions_dev3.c_lflag |= ICANON;
  /* commit the serial port settings */

//  cfmakeraw(&toptions_dev3);

  tcsetattr(dev3, TCSANOW, &toptions_dev3);

/*
  // open serial port
  dev4 = open("/dev/ttyUSB_3DR_receiver", O_RDWR | O_NOCTTY);
  // get current serial port settings
  tcgetattr(dev4, &toptions_dev4);
  // set 9600 baud both ways
  cfsetispeed(&toptions_dev4, B115200);
  cfsetospeed(&toptions_dev4, B115200);
  // 8 bits, no parity, no stop bits
  toptions_dev4.c_cflag &= ~PARENB;
  toptions_dev4.c_cflag &= ~CSTOPB;
  toptions_dev4.c_cflag &= ~CSIZE;
  toptions_dev4.c_cflag |= CS8;
  // Canonical mode
  toptions_dev4.c_lflag |= ICANON;
  // commit the serial port settings
  tcsetattr(dev4, TCSANOW, &toptions_dev4);
*/

  pthread_t thread_id_dev1;
  pthread_t thread_id_dev2;
  pthread_t thread_id_dev3;
  //pthread_t thread_id_dev4;

if (dev1 > 0)
{
  pthread_create(&thread_id_dev1, NULL, dev1_parser, NULL);
  printf("{\"frsky\":{\"STATE\":\"opened device with state: %d\"}}\n", dev1);
}
if (dev2 > 0)
{
  pthread_create(&thread_id_dev2, NULL, dev2_parser, NULL);
  printf("{\"orx\":{\"STATE\":\"opened device with state: %d\"}}\n", dev2);
}
if (dev3 > 3)
{
  pthread_create(&thread_id_dev3, NULL, dev3_parser, NULL);
  printf("{\"threeDR\":{\"STATE\":\"opened device with state: %d\"}}\n", dev3);
}
//if (dev4 == 3)
//{
//  printf("opened device 4\n");
  //pthread_create(&thread_id_dev4, NULL, dev4_parser, NULL);
//}
  pthread_exit(NULL);

  return 0;
}
