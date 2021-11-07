# Parallel Computing Assignment

### Mini-Project One: Conway's Game of Life

In order to run everything just run: make

Note that a random matrix is generated using the rows and columns values which are set in the makefile. Also remember to adjust the number of processes as well before running make. Don't forget that the number of processes must be a factor of the number of rows.

For this project my approach to parallelize the algorithm was as follows:

1) Reduce the two-dimensional board into multiple sub-boards using its rows. i.e decompose the matrix and hand each process it's own set of rows to work on.
2) Once a process works on their respective rows, we store the results in a one-dimensional array
3) We then use MPI_Allgather , to combine each processes rows into a one-dimensional vector.
4) Once each process has the next generation in a one-dimensional vector , we can convert it two a two-dimensional vector and continue onto the next generation.
5) This process is repeated for an amount of iterations that are passed in as arguments at compile time
6) The initial board is randomly generated and then used for the serial and parallel computations.
7) In the beginning of the parallel section , we allow process 0 to implement the serial implementatin which can be found in SerialConway.cpp. We time this process and we place a barrier after it since we want all the processes to begin execution at the same time.
8) One note to make is that the algorithm relies on the fact that , the number of rows divided by the number of processes has no remainder. i.e rows%processes = 0. If this is not the case, the program will print "`"Please ensure that rows/processes is an integer and not a float.`" and terminate.

### Mini-Project Two: Dijkstra's Algorithm

In order to run everything just run: make 

Note that a graph is initally randomly generated. You can set the number of vertices within the makefile before you run make. You should also set the number of threads/processes there.

#### OpenMP Implementation:

1. We first set the number of threads using omp_set_num_threads() since the number of threads are passed in as a parameter.
2. We then calculate the offset. The offset represents how many vertices will be allocated to each thread.
3. We then initialize the data structures for the problem and set the distance at the source vertex equal to 0.
4. The shared variables are the following : AdjacencyMatrix which is the actual matrix , distances which is a vector containing the distances from each vertex to the source, visited which is a vector denoting whether or not a vertex has been visited, the offset as explained above, the number of vertices in the graph , a variable MinDistance which will store the global minimum distance and a variable MinVertex which will store the global minimum vertex.
5. Each thread has private variables as follows:
   threadID which is used to identify each thread, startVertex which denotes the first vertex belonging to a thread and endVertex denotes the last vertex but is not inclusive in the threads set of vertices. Each thread will also have a localMinDistance variable and localMinVertex variable which will store the local mimimum distances and vertices respectively. Lastly all the iteration variables are private.
6. We start off by looping through each vertex.
7. We then have a single worksharing construct that will initalize the global minimum distance and global minimum vertex. These variables are MinDistance and MinVertex respectively.
8. Therafter each thread intializes the local variables localMinDistance and localMinVertex.
9. Then each thread loops through their set of vertices and tries to find a distance smaller than their localMinDistance. If they find such a distance and the vertex has not yet been visited, the localMinDistance and localMinVertex are updated.
10. We then have a critical section. This is used to update the global variables MinDistance and MinVertex to the  minimum of all local distances. The critical section ensures we avoid a race condition and that the global values are correct.
11. After the critical section , we place a barrier so that all the threads can wait and ensure that the global variables are fully updated before continuing.
12. We then allow process 0 to mark the global variable MinVertex as visited. We place another barrier after this since all calculations after this utilise the visited vector and we need to ensure it is updated before continuing.
13. Each thread then loops through their local set of vertices and update the distances vector.
14. We have another barrier since the next iteration uses the distances vector and it needs to be completely updated before utilising it again. Once each thread is done updating their local distances , we continue onto the next vertex and do so until all the vertices have been looped through.
15. We then return the distances array which will contain the shortest distance from the source to every other vertex.
16. After the parallel version is complete, we call the serial version and get the results and time it. We compare the serial result and the parallel result, if they are equal we output the times and the speedup.

#### MPI Implementation:

1. We calculate the offset which will represent the number of vertices that each process will work on.
2. We then calculate the start and end vertex for each thread. i.e the vertex it will start checking from and the last vertex which is not inclusive in the processes set of vertices.
3. We initialize the initial data structures and set the distance at the source equal to 0.
4. We have one outer loop which loops through all the vertices.
5. We then find the minimum distance in the distances array and mark the vertex as visited in the visited array.
6. Each process then loops through their set of vertices and updates their distances array if required.
7. We then call MPI_Allreduce for the distances array since in the next iteration each process will require the updated distances. We also set the operation as MPI_MIN since in each position , we want the smallest value.
8. We then continue onto the next iteration until completion.
9. At the end, we call the Serial version on the graph and time it. We then compare the serial result against our parallel result and it is the same , we output the Serial Time , parallel time and the speed up.
