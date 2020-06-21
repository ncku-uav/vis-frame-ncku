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


#define DEBUG 0

int main(int argc, char *argv[])
{
  int dev1, dev2, dev3, dev4, n, i;
  char buf_dev1[1024] = "temp text1";
  char buf_dev2[1024] = "temp text2";
  char buf_dev3[1024] = "temp text3";
  char buf_dev4[1024] = "temp text4";
  char rec_buf_dev1[1];
  struct termios toptions_dev1;
  struct termios toptions_dev2;
  struct termios toptions_dev3;
  struct termios toptions_dev4;
  size_t bytes_read;

  clock_t time_t;
  uint32_t get_interval = (CLOCKS_PER_SEC/1000)*500;

  setbuf(stdout, NULL);

//  dev1 = open("/dev/ttyUSB_orRX_receiver", O_RDWR | O_NOCTTY);
//  printf("dev1 opened as %i\n", dev1);

  /* wait for the Arduino to reboot */
  usleep(3500000);

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
  tcsetattr(dev1, TCSANOW, &toptions_dev1);

  /* open serial port */
  dev2 = open("/dev/ttyUSB_orRX_receiver", O_RDWR | O_NOCTTY);
  /* get current serial port settings */
  tcgetattr(dev2, &toptions_dev2);
  /* set 9600 baud both ways */
  cfsetispeed(&toptions_dev2, B57600);
  cfsetospeed(&toptions_dev2, B57600);
  /* 8 bits, no parity, no stop bits */
  toptions_dev2.c_cflag &= ~PARENB;
  toptions_dev2.c_cflag &= ~CSTOPB;
  toptions_dev2.c_cflag &= ~CSIZE;
  toptions_dev2.c_cflag |= CS8;
  /* Canonical mode */
  toptions_dev2.c_lflag |= ICANON;
  /* commit the serial port settings */
  tcsetattr(dev2, TCSANOW, &toptions_dev2);


while(1){
  time_t = clock();

  n = read(dev1, buf_dev1, 1024);
  /* insert terminating zero in the string */
  buf_dev1[n] = 0;
  //  printf("%i bytes read, buf_dev1fer contains: %s\n", n, buf_dev1);
  printf("%s",buf_dev1);

  n = read(dev2, buf_dev2, 1024);
  /* insert terminating zero in the string */
  buf_dev2[n] = 0;
  //  printf("%i bytes read, buf_dev1fer contains: %s\n", n, buf_dev1);
  printf("%s",buf_dev2);

  /* Send byte to trigger Arduino to send string back */
  //  write(dev1, "0", 1);
  /* Receive string from Arduino */


//  bytes_read = read( STDIN_FILENO, rec_buf_dev1, 1);
//  if(bytes_read >= 0){
//    rec_buf_dev1[ bytes_read ] = '\0';


//  scanf("%c", &rec_buf_dev1);
//  if (scanf("%c", &rec_buf_dev1) == 0)
//  {}

//    if(getchar()/*rec_buf_dev1*/ >0){//== 0x67){
/*    if(((uint32_t)time_t % get_interval) == 0){
      write(dev1, &rec_buf_dev1, 1);
      //usleep(10000);
      n = read(dev1, buf_dev1, 1024);
*/      /* insert terminating zero in the string */
/*      buf_dev1[n] = 0;
      //  printf("%i bytes read, buf_dev1fer contains: %s\n", n, buf_dev1);
      printf("%s",buf_dev1);
*/
    }
//  }

  if(DEBUG)
    {
      printf("Printing individual characters in buf_dev1 as integers...\n\n");
      for(i=0; i<n; i++)
	{
	  printf("Byte %i:%i, ",i+1, (int)buf_dev1[i]);
	}
      printf("\n");
    }

  return 0;
}
