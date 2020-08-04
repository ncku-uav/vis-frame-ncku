#!/bin/bash
BASEDIR=$(dirname "$0")

echo "copying files to destination..."

cp $BASEDIR/server_files/* /var/www/html

