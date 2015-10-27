CXX = g++
CC  = g++

CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g
LDFLAGS =   -g

SRCS1 = src/bmg.cpp src/text_tool.cpp
OBJS1 = $(subst .cpp,.o,$(SRCS1))

SRCS2 = src/ncgr.cpp src/ncer.cpp src/nscr.cpp src/lodepng.cpp src/image_tool.cpp
OBJS2 = $(subst .cpp,.o,$(SRCS2))

# Targets
PROGS = text_tool image_tool png_merge

all: $(PROGS)

# Targets rely on implicit rules for compiling and linking
text_tool: $(OBJS1)
		$(CXX) $(LDFLAGS) -o text_tool $(OBJS1) $(LDLIBS) 

image_tool: $(OBJS2)
		$(CXX) $(LDFLAGS) -o image_tool $(OBJS2) $(LDLIBS) 

png_merge: src/png_merge.o src/lodepng.o
		$(CXX) $(LDFLAGS) -o png_merge src/png_merge.o src/lodepng.o $(LDLIBS) 

# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f src/*.o $(PROGS)
