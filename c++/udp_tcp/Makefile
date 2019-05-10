# set compiler 
GCC = gcc -pthread #编译 pthread
G++ = g++ -pthread 

#set compiler switch
CFLAG = -Wall -c # -Wall显示所有警告，-g 可调试，-c编译

#set source tree
INCLUDE = ./include
SRC_DIR = ./src/
SRC_SD = ./src/send
SRC_REV = ./src/recv

SRC = $(wildcard $(SRC_DIR)/*.cpp)

SRC_C = $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))
       # foreach表示遍历$(D_SRC)的所有子目录同时把子目录下的.c文件遍历出来
OBJOUTPUT = ./obj/
BINOUTPUT = ./bin/
OBJS_SEND = $(patsubst %.cpp,%.o,$(SRC) $(SRC_SEND)) #patsubt cpp 直接转化成 .o
OBJS_DIR_RECV = $(addprefix $(OBJOUTPUT),$(notdir $(OBJS_RECV))) #addprefix  组合字符 添加前缀

%.o:%.cpp
	$(GXX) $(CFLAG) $< -I$(INCLUDE) -o $@
	mv $@ $(OBJOUTPUT)

send:$(OBJS_SEND)$
	$(GXX) $(OBJS_DIR_SEND) -o $(BINOUTPUT)$@

receive:$(OBJS_RECV)$
     $(GXX) $(OBJS_DIR_RECV) -o $(BINOUTPUT)$@

