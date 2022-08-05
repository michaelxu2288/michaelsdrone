#!/bin/bash

DIR=/home/pi/drone
NODE=/usr/bin/node

${DIR}/bin/led all 0
${DIR}/bin/led blue 1
echo "Arming motors\n"
${DIR}/bin/arm
echo "Armed"
${DIR}/bin/led all 0
echo "Starting up drone client \n"
${DIR}/bin/drone &
echo "Starting node server\n"
${NODE} ${DIR}/server/server.js &

