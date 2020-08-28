USE_CPP=1
DEBUG=0
OS := $(shell uname)
EXEC=libTest
OBJDIR=./obj/

ifeq ($(USE_CPP), 1)
CC=g++
else
CC=gcc
endif
LIBNAMESO=STrafficANPREngineAPI.so
CPP=g++ -std=c++11
NVCC=/usr/local/cuda-10.0/bin/nvcc
OPTS=-Ofast
LDFLAGS= -lm -pthread -ldl 
#LDFLAGS= -lm -pthread -ldl -L/home/s2w/__Dev/STrafficANPREngine
COMMON= -I./
#CFLAGS=-Wall -Wfatal-errors -Wno-unused-result -Wno-unknown-pragmas -fPIC
CFLAGS=-Wall -Wfatal-errors -Wno-unused-result -Wno-unknown-pragmas
CFLAGS+=$(OPTS)

ifneq (,$(findstring MSYS_NT,$(OS)))
LDFLAGS+=-lws2_32
endif

COMMON+= -DOPENCV
#CFLAGS+= -DOPENCV
LDFLAGS+= `pkg-config --libs opencv4 2> /dev/null || pkg-config --libs opencv`
COMMON+= `pkg-config --cflags opencv4 2> /dev/null || pkg-config --cflags`

OBJ=main.o AnprEV.o Camera.o CameraIP.o CameraUSB.o MessageQueue.o Plate.o 
OBJS = $(addprefix $(OBJDIR), $(OBJ))
DEPS = $(wildcard ./*.h) Makefile

all: $(OBJDIR) $(EXEC)

$(EXEC): $(OBJS)
	LD_LIBRARY_PATH=/home/s2w2/projects/Ubuntu $(CPP) -std=c++11 -pthread $(COMMON) $(CFLAGS) $^ -o $@ $(LDFLAGS) -L ./ /home/s2w2/projects/Ubuntu/$(LIBNAMESO)

$(OBJDIR)%.o: %.c $(DEPS)
	$(CC) $(COMMON) $(CFLAGS) -c $< -o $@

$(OBJDIR)%.o: %.cpp $(DEPS)
	$(CPP) -std=c++11 $(COMMON) $(CFLAGS) -c $< -o $@

$(OBJDIR)%.o: %.cu $(DEPS)
	$(NVCC) $(ARCH) $(COMMON) --compiler-options "$(CFLAGS)" -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJS) $(EXEC)
