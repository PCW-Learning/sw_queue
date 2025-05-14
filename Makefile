# 파일 경로 설정
SRC_DIR = src
INCLUDE_DIR = include
QUEUE_SRCS = $(wildcard $(SRC_DIR)/*.c)
QUEUE_OBJS = $(patsubst %.c, %.o, $(QUEUE_SRCS))
CFLAGS = -Wall -g -I$(INCLUDE_DIR)

# 데스크탑용 설정
DESKTOP_TARGET_LIB = libqueue_desktop.so
DESKTOP_CFLAGS = -Wall -g -fPIC -I$(INCLUDE_DIR)
DESKTOP_CC = gcc
INSTALL_INCLUDE_DIR = /usr/include
INSTALL_LIB_DIR = /usr/lib

# 구글테스트 관련 설정
GTEST_DIR = /home/pcw1029/projects/Learning/SW/googletest
GTEST_INCLUDE_DIR = $(GTEST_DIR)/googletest/include
GTEST_LIB_DIR = $(GTEST_DIR)/build/lib

FOR_GTEST_SRCS = $(wildcard $(SRC_DIR)/*.c)
FOR_GTEST_OBJS = $(patsubst %.c, %_gtest.o, $(FOR_GTEST_SRCS))
MY_GTEST_DIR = gtest
MY_GTEST_SRCS = $(MY_GTEST_DIR)/queue-gtest.cc
MY_GTEST_OBJS = $(patsubst %.cc, %.o, $(MY_GTEST_SRCS))
GTEST_TARGET = queue-gtest

# 변수 정의
CC = gcc
CXX = g++
GTEST_CFLAGS = -Wall -g -I$(INCLUDE_DIR) -I$(GTEST_INCLUDE_DIR) -std=c++11
GTEST_LDFLAGS = -L$(GTEST_LIB_DIR) -lgtest -lgtest_main -lpthread

# 기본 타겟
all: $(QUEUE_OBJS) 

# 구글테스트 빌드 및 실행
gtest: $(MY_GTEST_OBJS) $(FOR_GTEST_OBJS)
	$(CXX) $(GTEST_CFLAGS) -o $(GTEST_TARGET) $(MY_GTEST_OBJS) $(FOR_GTEST_OBJS) $(GTEST_LDFLAGS)

# desktop 타겟
desktop: $(QUEUE_SRCS)
	$(DESKTOP_CC) -shared $(DESKTOP_CFLAGS) -o $(DESKTOP_TARGET_LIB) $(QUEUE_SRCS) \
	&& echo "Copying libraries and headers to /usr/lib and /usr/include..." \
	&& sudo cp -v lib*.so $(INSTALL_LIB_DIR) \
	&& sudo cp -v include/*.h $(INSTALL_INCLUDE_DIR)
	
# 패턴 규칙: .c 파일을 .o 파일로 컴파일 (일반 빌드)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 패턴 규칙: .c 파일을 .gtest.o 파일로 컴파일 (구글테스트 빌드)
%_gtest.o: %.c
	$(CXX) $(GTEST_CFLAGS) -c $< -o $@

# 패턴 규칙: .cc 파일을 .o 파일로 컴파일 (구글테스트)
%.o: %.cc
	$(CXX) $(GTEST_CFLAGS) -c $< -o $@

# clean 타겟: 빌드 파일 정리
.PHONY: clean
clean:
	rm -f $(QUEUE_OBJS) $(FOR_GTEST_OBJS) $(MY_GTEST_OBJS) $(GTEST_TARGET) $(DESKTOP_TARGET_LIB)
