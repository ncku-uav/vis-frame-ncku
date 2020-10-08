#!/bin/bash

BASEDIR=$(dirname "$0")

sudo pkill websocketd

#echo "running target as user: www-data on localhost:8080"
#sudo -u www-data $BASEDIR/socket_c/websocketd --port 8080 $BASEDIR/socket_c/a.out

#echo 'running target as user: root on localhost:443'
#sudo nice -15 $BASEDIR/socket_c/websocketd --port 443 --devconsole $BASEDIR/socket_c/a.out

echo 'running target as user: root with websocketdBro'
#sudo $BASEDIR/socket_c/bro -m consumer -e webinterface & sudo $BASEDIR/socket_c/a.out | $BASEDIR/socket_c/bro -m publisher -e webinterface
sudo nice -14 $BASEDIR/socket_c/a.out | sudo nice -15 $BASEDIR/socket_c/bro -m publisher -e webinterface & sudo nice -13 $BASEDIR/socket_c/bro -m consumer -e webinterface
