CXX = g++

CXXFLAGS = -std=c++11 -Wall

OBJS = sim.o event.o stats.o min-heap.o queue.o

EXEC = simulator

TEXT_FILES =

simulator : $(OBJS)
				$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

sim.o : min-heap.hpp queue.hpp sim.hpp
event.o : event.hpp
min-heap.o :min-heap.hpp
queue.o : queue.hpp
clean :
				rm -f $(EXEC) $(TEXT_FILES) $(OBJS)