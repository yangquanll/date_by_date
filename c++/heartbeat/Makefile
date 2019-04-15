
#define compiler var.
GCC = gcc -pthread
GXX = g++ -pthread

#define compiler switch.
CFLAG = -Wall -c # -Wall显示所有警告，-g 可调试，-c编译 

#define header of files dir.
INCLUDE = ./include

#define compiler source code dir.
SRC_DIR = ./src
SRC_SD = ./src/send
SRC_RE = ./src/recv

SRC = $(wildcard $(SRC_DIR)/*.cpp) # wildcard 获取目录下文件
SRC_RECV = $(wildcard $(SRC_RE)/*.cpp)
SRC_SEND = $(wildcard $(SRC_SD)/*.cpp)

#define output dir for object files.
OBJOUTPUT = ./obj/

#define output dir for bin
BINOUTPUT = ./bin/

OBJS_SEND = $(patsubst %.cpp,%.o,$(SRC) $(SRC_SEND)) #patsubt cpp 直接转化成 .o
OBJS_RECV = $(patsubst %.cpp,%.o,$(SRC) $(SRC_RECV))

OBJS_DIR_SEND = $(addprefix $(OBJOUTPUT),$(notdir $(OBJS_SEND))) #notdir 去除路径信息 直接转化成 .O
OBJS_DIR_RECV = $(addprefix $(OBJOUTPUT),$(notdir $(OBJS_RECV))) #addprefix  组合字符即添加前缀

%.o:%.cpp
	$(GXX) $(CFLAG) $< -I$(INCLUDE) -o $@ 
	mv $@ $(OBJOUTPUT) 

# $@ = %.o (*.o) 命令行中$@代表的是目标文件也就是%.o,$<指的是生成目标文件的依赖文件也就是%.c

send:$(OBJS_SEND)$
	$(GXX) $(OBJS_DIR_SEND) -o $(BINOUTPUT)$@

receive:$(OBJS_RECV)$
	$(GXX) $(OBJS_DIR_RECV) -o $(BINOUTPUT)$@

clean:
	rm -rf $(OBJOUTPUT)*.o $(BINOUTPUT)*

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

all: send receive ECHO
.PHONY : all

