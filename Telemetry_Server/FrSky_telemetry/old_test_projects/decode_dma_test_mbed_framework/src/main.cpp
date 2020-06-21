#include "mbed.h"

RawSerial device(USBTX, USBRX);  // tx, rx

int main() {
    device.baud(9600);
    device.printf("Hello World\n");
}
