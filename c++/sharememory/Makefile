# set compiler 
GCC = gcc #-pthread 编译 pthread
G++ = g++ -pthread 

#set compiler switch
CFLAG = -Wall -c -g # -Wall显示所有警告，-g 可调试，-c编译

#set source tree 切勿这样 ./src/inc/ 后面不加'/'!!!!!
INCLUDE = ./src/inc
SRC_DIR = ./src/comm
SRC_SR = ./src/client
SRC_RR = ./src/server
Allcpp = $(shell find -name *.c) # 使用Linux 命令要用 $(shell cmd) =  AllDirs + foreach
AllDirs := $(shell ls -R | grep '^\./.*:$$' | awk '{gsub(":","");print}')


SRC = $(wildcard $(SRC_DIR)/*.c) # 带完整目录的 .cpp 
SRC_S = $(wildcard $(SRC_SR)/*.c)
SRC_R = $(wildcard $(SRC_RR)/*.c)

SRC_C = $(foreach dir, $(AllDirs), $(wildcard $(dir)/*.c))
       # foreach表示遍历$(D_SRC)的所有子目录同时把子目录下的.c文件遍历出来
OBJOUTPUT = ./obj/
BINOUTPUT = ./bin/
OBJS_SEND = $(patsubst %.c,%.o,$(SRC) $(SRC_S)) #patsubt cpp 直接转化成 .o
OBJS_RECV = $(patsubst %.c,%.o,$(SRC) $(SRC_R)) #patsubt cpp 直接转化成 .o

OBJS_DIR_SEND = $(addprefix $(OBJOUTPUT),$(notdir $(OBJS_SEND))) #notdir 除 路径信息 直接转化成 .O
OBJS_DIR_RECV = $(addprefix $(OBJOUTPUT),$(notdir $(OBJS_RECV))) #addprefix  组合字符 添加前缀

%.o:%.c
	$(GCC) $(CFLAG) $< -I$(INCLUDE) -o $@
	mv $@ $(OBJOUTPUT)

# $@ = %.o (*.o)
# $< ——第一个依赖文件
# gcc -o main.c = gcc -o $@
# $< ：代表规则中通过目录搜索得到的依赖文件列表的第一个依赖文件。
# $^： 代表所有通过目录搜索得到的依赖文件的完整路径名（目录 + 一般文件名）列表。
# $@：代表规则的目标



send:$(OBJS_SEND)$
	$(GCC) $(OBJS_DIR_SEND) -o $(BINOUTPUT)$@

receive:$(OBJS_RECV)$
	$(GCC) $(OBJS_DIR_RECV) -o $(BINOUTPUT)$@

clean:
	rm -rf $(OBJOUTPUT)*.o $(BINOUTPUT)* ./log_heartbeat

ECHO:
	@echo $(OBJS_SEND) 
	@echo "=========="
	@echo $(OBJS_RECV)
	@echo "=========="
	@echo $(OBJS_DIR_SEND)
	@echo "=========="
	@echo $(OBJS_DIR_RECV)
	@echo "=========="	
	@echo $(Allcpp)
	@echo "=========="
	@echo $(AllDirs)
	@echo "=========="
	@echo $(SRC_C)	
	@echo $(SRC_S)
	@echo $(SRC_R)
	@echo $(SRC)
	@echo $@

all: send receive ECHO

.PHONY : all
