#!/bin/bash

DIR=/home/pi/drone
NODE=/usr/bin/node

echo "Arming motors\n"
${DIR}/bin/arm
echo "Armed"
echo "Starting up drone client \n"
${DIR}/bin/drone &
echo "Starting node server\n"
${NODE} ${DIR}/server &

