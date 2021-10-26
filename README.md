# Parallel Computing Assignment

### Mini-Project One: Conway's Game of Life

For this project my approach to parallelize the algorithm was as follows:

1) Reduce the two-dimensional board into multiple sub-boards using its rows. i.e decompose the matrix and hand each process it's own set of rows to work on.
2) Once a process works on their respective rows, we store the results in a one-dimensional array
3) We then use MPI_Allgather , to combine each processes rows into a one-dimensional vector.
4) Once each process has the next generation in a one-dimensional vector , we can convert it two a two-dimensional vector and continue onto the next generation.
5) This process is repeated for an amount of iterations that are passed in as arguments at compile time
6) The initial board is randomly generated and then used for the serial and parallel computations.
7) In the beginning of the parallel section , we allow process 0 to implement the serial implementatin which can be found in SerialConway.cpp. We time this process and we place a barrier after it since we want all the processes to begin execution at the same time.
8) One note to make is that the algorithm relies on the fact that , the number of rows divided by the number of processes has no remainder. i.e rows%processes = 0. If this is not the case, the program will print "`"Please ensure that rows/processes is an integer and not a float.`" and terminate.
