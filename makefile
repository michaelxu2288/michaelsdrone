SRCS:=${wildcard src/*.cpp}
SRCDIR:=src

INCLUDEDIR:=include

OBJDIR:=build
OBJ_FILES := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

LIBS := ${wildcard ./lib/lib*.a}
CMD_LIBS := $(patsubst ./lib/lib%.a,-l%, ${LIBS})

OPTS:= -Iinclude -pthread -lpthread -std=c++2a -Wno-psabi -L./lib ${CMD_LIBS} -DENABLE_MOTOR

.PHONY: drone lib


all: drone
${OBJDIR}/%.o: ${SRCDIR}/%.cpp
	mkdir -p build
	g++ $^ -c -o $@ ${OPTS}

drone: ${OBJ_FILES} 
	mkdir -p bin
	g++ main/main.cpp $^ -o bin/drone ${OPTS}

%: ${OBJ_FILES} tools/%.cpp
	mkdir -p bin
	g++ $^ -o bin/$@ ${OPTS}
