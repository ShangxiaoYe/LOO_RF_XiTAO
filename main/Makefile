XITAO_PATH=/home/shangxiao/xitao-master
SHARK_PATH=/home/shangxiao/Shark-4.0.0/build
include $(XITAO_PATH)/makefile.sched
CXXFLAGS += -I$(XITAO_PATH)/include -I.
CXXFLAGS += -O3
CXXFLAGS += --std=c++11  ${EXTRAEINC}
CXX=g++
LIBS = -lm ${EXTRAELIBS} -L$(XITAO_PATH)/lib -lxitao -pthread -L$(SHARK_PATH)/lib -lshark -lboost_serialization
PROGRAM_OBJS=main.o 

all: main

%.o : %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

main: $(PROGRAM_OBJS)
	$(CXX) $^ -o $@ ${LIBS}

run: main
	./main 

clean:
	rm -rf main $(PROGRAM_OBJS) html latex
