#ifndef Dijkstra_h
# define Dijkstra_h
#include <vector>

using namespace std;


vector<int> SerialDijkstra(vector<vector<int>>& AdjacencyMatrix, int source,int vertices);
int minDistance(vector<int>&distances,vector<bool>&visited,int vertices);
void ReadGraphFromTextFile(vector<vector<int>>&AdjacencyMatrix,int vertices);
#endif