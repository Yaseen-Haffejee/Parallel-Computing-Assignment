#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

double GraphDensity(int vertices,int edges ){
    return (edges/(vertices*(vertices-1)));
}

int GenerateRandomWeight(){
    return rand()%20;
}

int RandomEdge(){
    return rand()%2;
}

vector<vector<int>> GenerateGraph(int vertices){
    vector<vector<int>> Graph (vertices,vector<int>(vertices));
    for(int i=0;i<vertices;i++){

        for(int j=0;j<vertices;j++){
            if(RandomEdge()){
                if(i ==j){
                    Graph[i][j] = 0;
                }
                else{
                    Graph[i][j] = GenerateRandomWeight();
                }
            }
            else{
                Graph[i][j] = 0;
            }
        }
    }
    return Graph;
}

void PrintGraph(vector<vector<int>>& Graph,int vertices){
    ofstream MyFile("Graph.txt");
    MyFile << vertices <<endl;
    for(int i=0;i<vertices;i++){
        for(int j=0;j<vertices;j++){
            MyFile<<Graph[i][j]<<" ";
        }
        MyFile<<endl;
    }

}

int main(int argc, char * argv[]){

    int numberOfVertices = atoi(argv[1]);
    vector<vector<int>> Graph = GenerateGraph(numberOfVertices);
    PrintGraph(Graph,numberOfVertices);

    return 0;
}