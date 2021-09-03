SRCS:=${wildcard src/*.cpp}

.PHONY: drone arm calibration low_speed high_speed pressure accel

all: drone

rampup: ${SRCS} tools/rampup.cpp
	mkdir -p bin
	g++ tools/rampup.cpp ${SRCS} -o bin/rampup -Iinclude -Isrc/include -pthread -lpthread -L./3rdparty -li2c -std=c++11

high_speed: ${SRCS} tools/high_speed.cpp
	mkdir -p bin
	g++ tools/high_speed.cpp ${SRCS} -o bin/high_speed -Iinclude -Isrc/include -pthread -lpthread -L./3rdparty -li2c -std=c++11

low_speed: ${SRCS} tools/low_speed_test.cpp
	mkdir -p bin
	g++ tools/low_speed_test.cpp ${SRCS} -o bin/low_speed -Iinclude -Isrc/include -pthread -lpthread -L./3rdparty -li2c -std=c++11

arm: ${SRCS} tools/arm.cpp
	mkdir -p bin
	g++ tools/arm.cpp ${SRCS} -o bin/arm -Iinclude -Isrc/include -pthread -lpthread -L./3rdparty -li2c -std=c++11

calibration: ${SRCS} tools/calibrate.cpp
	mkdir -p bin
	g++ tools/calibrate.cpp ${SRCS} -o bin/calibrate -Iinclude  -Isrc/include -pthread -lpthread -L./3rdparty -li2c -std=c++11

pulse: ${SRCS} tools/pulse.cpp
	mkdir -p bin
	g++ tools/pulse.cpp ${SRCS} -o bin/pulse -Iinclude  -Isrc/include -pthread -lpthread -L./3rdparty -li2c -std=c++11

pressure: ${SRCS} tools/pressure.cpp
	mkdir -p bin
	g++ tools/pressure.cpp ${SRCS} -o bin/pressure -Iinclude  -Isrc/include -pthread -lpthread -L./3rdparty -li2c -std=c++11

accel: ${SRCS} tools/accl_test.cpp
	mkdir -p bin
	g++ tools/accl_test.cpp ${SRCS} -o bin/accel -Iinclude  -Isrc/include -pthread -lpthread -L./3rdparty -li2c -std=c++11

setmotors: ${SRCS} tools/set_motors.cpp
	mkdir -p bin
	g++ tools/set_motors.cpp ${SRCS} -o bin/setmotors -Iinclude  -Isrc/include -pthread -lpthread -L./3rdparty -li2c -std=c++11


drone: 
	mkdir -p bin
	g++ main/main.cpp ${SRCS} -o bin/drone -Iinclude -Isrc/include -pthread -lpthread -L./3rdparty -li2c -std=c++11