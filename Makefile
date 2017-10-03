CXX = g++
DEBUG = -g -D DEBUG
TARGET = ./build/main
SRCS = line.cpp matrix.cpp test/test3d.cpp
INCLUDE_DIR = .
CFLAGS = -Wall $(INCLUDE_DIR:%=-I%)
MAIN = 3d

all: $(MAIN)
$(MAIN): $(SRCS)
	$(CXX) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	$(RM) *.o $(TARGET)