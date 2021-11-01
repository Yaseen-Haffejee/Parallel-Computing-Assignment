#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits.h>

using namespace std;

void ReadGraphFromTextFile(vector<vector<int>>&AdjacencyMatrix,int vertices){
    ifstream Matrix("Graph.txt");
    string line;
    int i = 0,j = 0, k= 0 ;
    while(getline(Matrix,line)){
        stringstream S(line);
        string val;
        if(k == 0){
            k++;
            continue;
        }
        while(S>>val){
            if(j>= vertices){
                continue;
            }
            AdjacencyMatrix[i][j] = stoi(val);
            j++;
        }
        i++;
        j = 0;
    }
}

void PrintGraph(vector<vector<int>>&AdjacencyMatrix,int vertices){
    for(int i=0;i<vertices;i++){
        for(int j=0;j<vertices;j++){
            cout<<AdjacencyMatrix[i][j]<<" ";
        }
        cout<<endl;
    }
}
int minDistance(vector<int>&distances,vector<bool>&visited,int vertices){

    int minDistance = INT_MAX;
    int position;
    // loop through all the distances and if the corresponding node is unvisited with a shorter distance, update minDistance
    for(int k=0;k<vertices;k++){
        if(distances[k]<=minDistance && visited[k]==false){
            minDistance = distances[k];
            position = k;
        }
    }
    return position;

}
vector<int> SerialDijkstra(vector<vector<int>>& AdjacencyMatrix, int source,int vertices){
    vector<int> distances(vertices,INT_MAX);
    vector<bool> Visited(vertices,false);
    vector<int> Nodes(vertices);
    for(int i=0;i<vertices;i++){
        Nodes[i] = i;
    }
    distances[source] = 0;
    int length = Nodes.size();
    while(length != 0){
        int u = minDistance(distances,Visited,vertices);
        length--;
        Visited[u] = true;
        for(int i = 0;i<vertices;i++){
             if (!Visited[i] && AdjacencyMatrix[u][i] && distances[u] != INT_MAX && distances[u] + AdjacencyMatrix[u][i] < distances[i]){
                        distances[i] = distances[u] + AdjacencyMatrix[u][i];
                }
                
        }
    }


    return distances;
}
vector<int> serialDijkstra(int argc, char * argv[]){
    // Number of vertices passed in as the first agument 
    int NumberOfVertices = atoi(argv[1]);
    int source = atoi(argv[2]);
    // Declare 2d matrix that will be the adjacency matrix
    vector<vector<int> > AdjacencyMatrix(NumberOfVertices,vector<int>(NumberOfVertices));
    //read from the file Graph.txt and store the adjacency matrix
    ReadGraphFromTextFile(AdjacencyMatrix,NumberOfVertices);
    vector<int> dis = SerialDijkstra(AdjacencyMatrix,source,NumberOfVertices);
    return dis;
}