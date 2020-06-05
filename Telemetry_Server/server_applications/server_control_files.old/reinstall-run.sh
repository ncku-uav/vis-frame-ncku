#!/bin/bash

BASEDIR=$(dirname "$0")

#sudo chown www-data /dev/ttyUSB_frsky_receiver
#sudo chown www-data /dev/ttyUSB_orRX_receiver
#sudo chown www-data /dev/ttyUSB_3DR_receiver

sudo $BASEDIR/install.sh && $BASEDIR/run.sh
