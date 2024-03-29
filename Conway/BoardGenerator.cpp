#include <iostream>
#include <vector>
#include <fstream>
#include "Conway.h"


using namespace std;

// int rows;
// int columns;
// method to randomly generate the initial board
// Live cells are denoted by a 1 and dead cells are denoted by 0.
vector<vector<int>> GenerateBoard(int rows, int columns){
    // create a matrix with rows number of rows and columns number of columns
    vector<vector<int>> board(rows,vector<int>(columns));
    for(int i =0; i<rows;i++){
       for(int j =0; j<columns;j++){
           board [i][j] = rand()%2;
       }
    }
    return board;
}

// void PrintBoard(vector<vector<int>>&board){
//     ofstream Matrix("Matrix.txt");
//     for(int i =0; i<rows;i++){
//         Matrix<<"{";
//        for(int j =0; j<columns;j++){
//          Matrix<<board[i][j]<<", ";
//        }
       
//        Matrix<<"},\n";
//     }
//     Matrix.close();
// }
void PrintBoard(vector<vector<int>>&board, int rows, int columns){
    for(int i =0; i<rows;i++){
       for(int j =0; j<columns;j++){
           cout<<board[i][j]<<" ";
       }
       cout<<"\n";
    }
    cout<<endl;
}

// int main(int argc, char* argv[]){
//     rows = stoi(argv[1]);
//     columns = stoi(argv[2]);
//     vector<vector<int>> b =GenerateBoard(rows,columns);
//     // PrintBoard(b);
// }
 