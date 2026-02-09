CXX = g++

CXXFLAGS = -std=c++11 -Wall

OBJS = customer.o min-heap.o queue.o main.o stats.o sim.o

EXEC = run

TEXT_FILES =

run: $(OBJS)
				$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

sim.o : sim.hpp
main.o : sim.hpp
customer.o : customer.hpp
min-heap.o : min-heap.hpp
queue.o : queue.hpp
stats.o : stats.hpp

clean :
				rm -f $(EXEC) $(TEXT_FILES) $(OBJS)