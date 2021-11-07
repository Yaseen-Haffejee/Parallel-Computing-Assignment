#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <omp.h>
#include "Dijkstra.h"

using namespace std;

vector<int> ParallelDijkstra(vector<vector<int>>&AdjacencyMatrix,int vertices,int source,int processes){
    // number of rows that each process will have of the adjacency matrix
    int offset = vertices/processes;
    // set the number of threads to the incoming number of processes
    omp_set_num_threads(processes);

    vector<int>distances(vertices,INT_MAX);
    vector<bool>visited(vertices,false);
    vector<int>Nodes(vertices);
    for(int i =0;i<vertices;i++){
        Nodes[i] = i;
    }

    distances[source] = 0;
    int MinDistance;
    int MinVertex;
    // start of parallel region
    // offset and adjacency matrix are shared since each thread will use it but never update it
    #pragma omp parallel shared(AdjacencyMatrix,visited,distances,MinDistance,vertices,MinVertex,offset) 
    {
        int threadID = omp_get_thread_num();
        int startVertex =  threadID * offset;
        int endVertex = startVertex + offset;
        for(int i = 0; i < vertices-1;i++){
            //allow a single thread to initialize the global variables
            #pragma omp single
            {
                MinDistance = INT_MAX;
                MinVertex = -1;
            }
            //initialize the local minimum distnace and local minimum vertex
            int localMinDistance =  INT_MAX;
            int localMinVertex = -1;
            // loop through all the vertices assigned to a thread and if there's a distance shorter, updated the local min
            for(int k=startVertex;k<endVertex;k++){
                if(distances[k]<localMinDistance && !visited[k]){
                    localMinDistance = distances[k];
                    localMinVertex = k;
                }
            }
            // critical section since each thread needs to come in an update the global min to be the smallest of the local minimums
            #pragma omp critical
            {
                if(localMinDistance < MinDistance){
                    MinDistance = localMinDistance;
                    MinVertex = localMinVertex;
                }
            }
            // place a barrier so that when all threads reach here, the global min is updated completely
            #pragma omp barrier 
            // let thread zero mark the new min vertex as visited
            if(threadID ==0){
                if(MinVertex != -1){
                    visited[MinVertex] = 1;
                }
            }
            // all threads wait for thread 0
            #pragma omp barrier  
            if(MinVertex != -1){
                // loop through all the vertices and update the distances vector accordingly
                for(int j= startVertex;j<endVertex;j++){
                    if(!visited[j] && AdjacencyMatrix[MinVertex][j] && distances[MinVertex] != INT_MAX && distances[MinVertex] + AdjacencyMatrix[MinVertex][j] < distances[j]){

                        distances[j] = distances[MinVertex] + AdjacencyMatrix[MinVertex][j];
                    }
                }
            }
            // only continue once all the threads have finished updating distances if they had to 
            #pragma omp barrier
        }

    }
    return distances;

}
