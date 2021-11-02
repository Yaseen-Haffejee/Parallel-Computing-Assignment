#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <chrono>
#include "Dijkstra.h"
#include "mpi.h"

using namespace std;

int main(int argc, char * argv[]){
    int processID;
    MPI_Init(NULL,NULL);
    // number of vertices is first argument
    int NumberOfVertices = atoi(argv[1]);
    // source vertex is the second argument
    int source = atoi(argv[2]);
    // number of processes is the third argument
    int NumProcesses = atoi(argv[3]);
    vector<vector<int>> AdjacencyMatrix(NumberOfVertices,vector<int> (NumberOfVertices));
    // actually reading the graph
    ReadGraphFromTextFile(AdjacencyMatrix,NumberOfVertices);
    // number of vertices per process
    int offset = NumberOfVertices/NumProcesses;

    // get the rank of each process
    MPI_Comm_rank(MPI_COMM_WORLD,&processID);
    // calculate the range of vertices each process will calculate the local minimum distances for
    int startVertex = processID*offset;
    int endVertex = startVertex + offset;
    vector<int> distances(NumberOfVertices,INT_MAX);
    // Temp distances will be used to collect the result from MPI_Allreduce 
    vector<int> Tempdistances(NumberOfVertices);
    vector<bool>Visited(NumberOfVertices,false);
    auto start = MPI_Wtime();
    // always initalize the distance at the source to be zero
    distances[source] = 0;
    int u;
    // loop through all the vertices
    for(int i = 0;i<NumberOfVertices - 1;i++){
        // find the vertex with the minimum cost
        u = minDistance(distances,Visited,NumberOfVertices);
        // mark that vertex as visited
        Visited[u] = true;
        // each process will loop through their range of vertices and see if their local distances need to be updated
        for(int j = startVertex;j<endVertex;j++){
            if (!Visited[j] && AdjacencyMatrix[u][j] && distances[u]!= INT_MAX && distances[u] + AdjacencyMatrix[u][j] < distances[j]){
                //update the local distances
                distances[j] = distances[u] + AdjacencyMatrix[u][j];
            }
        }
        // Each process sends their distances vector, and we take the min from each index and accumulate it all into temp distances
        MPI_Allreduce(&distances[0],&Tempdistances[0],NumberOfVertices,MPI_INT,MPI_MIN,MPI_COMM_WORLD);
        // update distances 
        distances = Tempdistances;
    }
    auto end = MPI_Wtime();
    // end timing and get total time
    auto ParallelTime = end - start;
    double TotalTime;
    MPI_Reduce(&ParallelTime,&TotalTime,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if(processID == 0){
        double AverageParallelTime = TotalTime/NumProcesses;
        // start timing the serial portions
        auto SerialStart = MPI_Wtime();
        // call the serial dijkstra and time how long Serial Takes to compute result
        vector<int> SerialResults = SerialDijkstra(AdjacencyMatrix,source,NumberOfVertices);
        auto SerialEnd = MPI_Wtime();
        auto SerialTime = SerialEnd - SerialStart;
        cout<<"-------------------------------------------------------------------------\n";
        cout<<"For a graph with "<< NumberOfVertices<<" vertices \n";
        cout<< "The source vertex is: "<<source<<endl;
        cout<<"The number of processes used is: "<<NumProcesses<<endl;
        cout<<endl;
        // check if the serial Result (distances calculated in serial) is the same as the parallel
        if(distances == SerialResults){
            cout<<"The Serial and Parallel Results are Equal \n";
            cout<<"The Serial Time is: "<<SerialTime<<endl;
            cout<<"The Parallel Time is: "<<AverageParallelTime<<endl;
            cout<<"The Speedup is: "<<(SerialTime/AverageParallelTime)<<endl;
        }
        else{
            cout<<"The Serial and Parallel Results are not equal \n\n";
        }
        cout<<"-------------------------------------------------------------------------\n";
    }
    MPI_Finalize();
    return 0;
}