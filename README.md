# Priority-Queue-Simulation-Project

This program is a simulation of a service center, conducted using event-driven programming,
a priority queue, and a FIFO queue.
The purpose of the simulation is to be able to test a customer service system given
only 3 parameters, the average number of events per time unit (lambda), the average
number of services that can be completed in 1 time unit (mu), and the number of service 
channels (M). Lambda and mu can be easily found with the Poisson distribution
and M can be varied to optimize for the necessary throughput. We can also use this
simulation to verify the predictions of the analytical model.

The simulation will return 5 simulated values that can be used to determine the efficiency
of the service center: Po, W, Wq, Rho, and the chance of having to wait

Po = The percent idle time, Po; that is, the percentage of time that no one is in the system

W = The average time a customer spends in the system.  (the time the customer spent in line plus the time spent getting service)

Wq = The average time a customer spends waiting in the queue

Rho = The utilization factor for the system, (rho); that is, the proportion of the system's resources which is used by the traffic which arrives.

Chance of having to wait = The probability that any given arrival will have to wait in line before recieving service

These first 4 values will be compared to those that are calculated by the analytical
model given the same lambda, mu, and M. The analytical model will also display 2 
additional measures: L and Lq.

L = The average number of people in the system (the number in line plus the number being served.)

Lq = The average numbers of customers in the queue

Formulas for the analytical values are contained in the stats.cpp getStatistics() function
The simulated and analytical values will be displayed in a table after the simulation completes

# How to Use

This program comes with a Makefile, to use it run the command "make" in the command line interface.
This will generate an executable called "run".
Verify that the data files "test1.txt" and "test2.txt" are in the same directory as the executable.
Verify that the test1.txt and test2.txt contains the necessary values to seed the simulation in the
following format:

"//FILE BEGIN (DO NOT INCLUDE THIS LINE)
lambda (integer)
mu (integer)
M (integer)
EVENTS_TO_SIMULATE (integer)"

Issue the command "./run" in the command line to run the executable.
No user interaction is required beyond this point and the program will automatically run two simulations,
one from each of the files, and then display the results as previously described.
The program will load the files, process EVENTS_TO_SIMULATE number of events
through the priority queue and FIFO queue then gather, process, and display the data.
Simply change the values in each of the test files and issue the "./run" command again
to run the simulation with new values.
