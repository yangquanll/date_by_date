#objects =  heartbeatMonitor_send.o udpnet.o Log.o commonfun.o
#revobj = heartbeatMonitor.o Log.o commonfun.o udpnet.o
#recv : $(revobj)
#	g++ -W -Wall -pthread -o re$(revobj)
#send : $(objects) 
#	g++ -W -Wall -pthread -o send$(objects)
#$(objects) : .h # 都依赖print.h
#	mian.o : mian.c  #干掉了gcc -c main.c 让Gun make自动推导了。
#	print.o : print.c 　　　　
#clean :　　　　　　　　　　
#	rm send $(objects) recv $(revobj)

#define compiler var.
GCC = gcc -pthread
GXX = g++ -pthread

#define compiler switch.
CFLAG = -Wall -c 

#define header of files dir.
INCLUDE = ./include

#define compiler source code dir.
SRC_DIR = ./src
SRC_SD = ./src/send
SRC_RE = ./src/recv

SRC = $(wildcard $(SRC_DIR)/*.cpp)
SRC_RECV = $(wildcard $(SRC_RE)/*.cpp)
SRC_SEND = $(wildcard $(SRC_SD)/*.cpp)
#define output dir for object files.
OBJOUTPUT = ./obj/

#define output dir for bin
BINOUTPUT = ./bin/

OBJS_SEND = $(patsubst %.cpp,%.o,$(SRC) $(SRC_SEND))
OBJS_RECV = $(patsubst %.cpp,%.o,$(SRC) $(SRC_RECV))

OBJS_DIR_SEND = $(addprefix $(OBJOUTPUT),$(notdir $(OBJS_SEND)))
OBJS_DIR_RECV = $(addprefix $(OBJOUTPUT),$(notdir $(OBJS_RECV)))

%.o:%.cpp
	$(GXX) $(CFLAG) $< -I$(INCLUDE) -o $@
	mv $@ $(OBJOUTPUT)

send:$(OBJS_SEND)$
	$(GXX) $(OBJS_DIR_SEND) -o $(BINOUTPUT)$@

receive:$(OBJS_RECV)$
	$(GXX) $(OBJS_DIR_RECV) -o $(BINOUTPUT)$@

clean:
	rm -f $(OBJOUTPUT)*.o

ECHO:
	@echo $(OBJS_SEND) 
	@echo "=========="
	@echo $(OBJS_RECV)
	@echo "=========="
	@echo $(OBJS_DIR_SEND)
	@echo "=========="
	@echo $(OBJS_DIR_RECV)
	@echo "=========="	
	@echo $@

