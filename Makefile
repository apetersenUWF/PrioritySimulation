CXX = g++

CXXFLAGS = -std=c++11 -Wall

OBJS = customer.o min-heap.o queue.o main.o

EXEC = run

TEXT_FILES =

run: $(OBJS)
				$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

main.o : min-heap.hpp queue.hpp
customer.o : customer.hpp
min-heap.o : min-heap.hpp
queue.o : queue.hpp
clean :
				rm -f $(EXEC) $(TEXT_FILES) $(OBJS)