#!/bin/bash
BASEDIR=$(dirname "$0")

echo "copying files to destination..."

cp $BASEDIR/../web_server_files/* /var/www/html

