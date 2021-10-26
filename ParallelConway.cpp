#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <assert.h>
#include "Conway.h"
#include <mpi.h>

using namespace std;

// please set the initial matrix here
// We just copy the matrix from matrix.txt which will be generated when we run BoardGenerator

// Method that will print each generation to a file
void PrintBoard(vector<vector<int>>&board, int iteration,bool P,int rows, int columns){
    ofstream Matrix("ParallelResult.txt", ios_base::app);
    Matrix << "The Parallel Result after iteration " <<iteration << " is: \n";
    for(int i =0; i<rows;i++){
        Matrix<<"{";
       for(int j =0; j<columns;j++){
         Matrix<<board[i][j]<<", ";
       }

       Matrix<<"},\n";
    }
    Matrix<<"\n";
    Matrix.close();
}
void Print(vector<vector<int>>&b){
    for(auto a : b){
        for(auto x : a){
            cout<<x<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}
void Print(vector<int>&m){
    for(auto x : m){
        cout<<x<<" ";
    }
    cout<<endl;
}
void Print(vector<int>m,int rows, int columns){
    int r = 1;
    for(int i=0 ; i<rows*columns;i+=columns ){
        vector<int> tmp;
        for(int j =i;j<columns*r;j++){
            cout<<m[j]<<", ";
        }
        cout<<endl;
        r++;
    }
    cout<<endl;
}
// Method to convert 2d vector into 1d vector for broadcasting
vector<int> convert2dTo1D(vector<vector<int>>&board,int rows,int columns){
    vector<int> Board(rows*columns);
    int pos = 0;
    for(int i =0;i<rows;i++){

        for(int j=0;j< columns; j++){
            Board[pos] = board[i][j];
            pos++;
        }
    }
    return Board;
}
// method to convert 1d vector into 2d vector for ease of checking neighbours
vector<vector<int>> convert1dT2D(vector<int> ptr,int rows,int columns){
    vector<vector<int>> Board;
    int r=1;
    for(int i=0 ; i<rows*columns;i+=columns ){
        vector<int> tmp;
        for(int j =i;j<columns*r;j++){
            tmp.push_back(ptr[j]);
        }
        r++;
        Board.push_back(tmp);
    }
    return Board;
}
// Get the number of neighbours that are alive given a board and the position on the board
int NumAliveNeighours(vector<int>&board,int row,int col, int rows, int columns){
    int startIndex =  row * rows;
    int Position = startIndex + col;
    int count = 0;
    int actualRows = rows - 1;
    int actualCols = columns - 1;
    int left = 0, right = 0, up = 0, down = 0,diagonalLeftUp = 0, diagonalRightUp = 0, diagonalLeftDown = 0, diagonalRightDown = 0;
    if(col > 0){
        left = board[Position - 1];
    }
    if(col<actualCols){
        right = board[Position + 1];
    }
    if(row > 0){
        int pos = ((row-1)*rows) + col;
        up  = board[pos];
    }
    if(row < actualRows){
        int pos = ((row+1)*rows) + col ;
        down =  board[pos];
    }
    if(col < actualCols && row >0){
        int pos = ((row-1)*rows) +(col+1);
        diagonalRightUp =  board[pos];
    }
    if(col > 0 && row >0){
        int pos = ((row-1)*rows) +(col-1);
        diagonalLeftUp = board[pos];
    }
    if(col < actualCols && row<actualRows){
        int pos = ((row+1)*rows) +(col+1);
        diagonalRightDown = board[pos];
    }
    if(col > 0 && row <actualRows){
        int pos = ((row+1)*rows) +(col-1);
        diagonalLeftDown =  board[pos];
    }
    if(up){
        count ++;
    }
    if(down){
        count ++;
    } 
    if(left){
        count ++;
    }
    if(right){
        count ++;
    }
    if(diagonalRightUp){
        count ++;
    }
    if(diagonalRightDown){
        count ++;
    } 
    if(diagonalLeftUp){
        count ++;
    } 
    if(diagonalLeftDown){
        count ++;
    }

    return count;
    
}
// Get the number of neighbours that are alive given a board and the position on the board
int NumberOfLiveNeighbours(vector<vector<int>>&board,int row,int col, int rows, int columns,bool p){
    int numberAlive = 0;
    if(row==0 && col ==0){
        // the only possible neighbours are : right,down and diagonal Right
        int right =  board[row][col+1];
        int down = board[row+1][col];
        int diagRight = board[row+1][col+1];
        if(right)
            numberAlive++;
        if(down)
            numberAlive++;
        if(diagRight)
            numberAlive++;

    }
    else if(row==0 && col==columns-1){
         // the only possible neighbours are : left,down and diagonal left
        int left =  board[row][col-1];
        int down = board[row+1][col];
        int diagleft = board[row+1][col-1];
        if(left)
            numberAlive++;
        if(down)
            numberAlive++;
        if(diagleft)
            numberAlive++;
    }
    else if(row==rows-1 && col==0){
        // possible moves: right, up, diagonal right
        int right =  board[row][col+1];
        int up = board[row-1][col];
        int diagRight = board[row-1][col+1];
        if(right)
            numberAlive++;
        if(up)
            numberAlive++;
        if(diagRight)
            numberAlive++;
    }
    else if(row == rows-1 && col == columns-1){
        // possible moves: left, up, diagonal left
        int left =  board[row][col-1];
        int up = board[row-1][col];
        int diagleft = board[row-1][col-1];
        if(left)
            numberAlive++;
        if(up)
            numberAlive++;
        if(diagleft)
            numberAlive++;
    }
    else if(row >0 && row < rows-1 && (col == 0 || col == columns-1)){
        // possible moves: up, down regardless of column
        int up = board[row-1][col];
        int down = board[row+1][col];
        int right = 0,left = 0,diagRightUp = 0,diagRightDown = 0,diagLeftUp = 0,diagLeftDown = 0;
        if(col == 0){
            // possible moves now: up,down,right,right diagonal up and down
            right = board[row][col+1];
            diagRightUp = board[row - 1][col + 1];
            diagRightDown = board[row + 1][col + 1];
        }
        else if(col == columns-1){
            // possible moves now: up,down,left,left diagonal up and down
            left = board[row][col-1];
            diagLeftUp = board[row-1][col-1];
            diagLeftDown = board[row+1][col-1];
        }
        if(down)
            numberAlive++;
        if(up)
            numberAlive++;
        if(right)
            numberAlive++;
        if(left)
            numberAlive++;
        if(diagRightDown)
            numberAlive++;
        if(diagRightUp)
            numberAlive++;
        if(diagLeftUp)
            numberAlive++;
        if(diagLeftDown)
            numberAlive++;
    }
    else if(row ==0 && (col<columns-1 || col>0)){
        //possible moves : down, left ,right, diag left down and diag right down
        int down = board[row+1][col];
        int left =  board[row][col-1];
        int right = board[row][col+1];
        int diagLeftDown = board[row+1][col-1];
        int diagRightDown = board[row + 1][col + 1];
        if(down)
            numberAlive++;
        if(right)
            numberAlive++;
        if(left)
            numberAlive++;
        if(diagRightDown)
            numberAlive++;
        if(diagLeftDown)
            numberAlive++;
    }
    else if( row == rows-1 && (col<columns-1 || col>0)){
        //possible moves : down, left ,right, diag left up and diag right up
        int left =  board[row][col-1];
        int right = board[row][col+1];
        int up = board[row-1][col];
        int diagRightUp = board[row - 1][col + 1];
        int diagLeftUp = board[row-1][col-1];
        if(up)
            numberAlive++;
        if(right)
            numberAlive++;
        if(left)
            numberAlive++;
        if(diagRightUp)
            numberAlive++;
        if(diagLeftUp)
            numberAlive++;
    }
    else{
        // all moves are possible
        int up = board[row-1][col];
        int down = board[row+1][col];
        int left =  board[row][col-1];
        int right = board[row][col+1];
        int diagLeftDown = board[row+1][col-1];
        int diagRightDown = board[row + 1][col + 1];
        int diagRightUp = board[row - 1][col + 1];
        int diagLeftUp = board[row-1][col-1];
        if(down)
            numberAlive++;
        if(up)
            numberAlive++;
        if(right)
            numberAlive++;
        if(left)
            numberAlive++;
        if(diagRightDown)
            numberAlive++;
        if(diagRightUp)
            numberAlive++;
        if(diagLeftUp)
            numberAlive++;
        if(diagLeftDown)
            numberAlive++;
    }
    return numberAlive;
}
int main(int argc, char * argv[]){
    // the number of processes being used, as well as the rows and columns of the matrix are passed in as arguments
    int NumProcesses =  atoi(argv[1]);
    int rows =  atoi(argv[2]);
    int columns = atoi(argv[3]);
    int iters = atoi(argv[4]);
    vector<vector<int> > Board = GenerateBoard(rows,columns);
    // measuring the performance
    // auto start = chrono::high_resolution_clock::now();
    MPI_Init(&argc, &argv);
    // processID will identify each process and offset will determine how many rows of the matrix are being processed per process
    int processID,offset,next;
    double TotalTime,SerialTime;
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);
    if(processID ==0 ){
        SerialTime = SerialConway(argc,argv,Board);
        
    }
    MPI_Barrier(MPI_COMM_WORLD);
    vector<vector<int>> board(Board);
    // auto start = MPI_Wtime();
    offset = rows/NumProcesses;
    // arr will be used to store the 1d representation of the matrix
    vector<int> arr(rows*columns);
    // startPos and endPos are where we start iterating from and end iterating in the matrix
    // i.e they represent the first row and endPos-1 represents the last row that a process will compute
    int startPos = 0;
    int endPos = offset;
    if(processID != 0){
        startPos = processID * offset;
        endPos = startPos + offset;
    }
    auto start = MPI_Wtime();
    // this outer for loop determines how many generations we run for
    for(int iteration=0;iteration<iters;iteration++){
        // need to reset the board to be the new Generation 
        if(iteration >=1 && processID != 0){
            board = convert1dT2D(arr,rows,columns);
        }
        // The Result of our computation, i.e determining if cells are alive or dead for a processes respective rows are stored in Result
        vector<int>Result;
        for(int i = startPos; i <endPos; i++){
            for(int j = 0; j < columns; j++){
                // int index = (i*columns)+ j;
                // int value = arr[index];
                int value = board[i][j];
                int aliveNeighbours = NumberOfLiveNeighbours(board,i,j,rows,columns,true);
                // int aliveNeighbours = NumAliveNeighours(arr,i,j,rows,columns);
                // if the cell is alive with two or three neighbours, it remains alive
                if(value==1 &&(aliveNeighbours ==2 || aliveNeighbours ==3)){
                    Result.push_back(1);
                }
                // if the cell is dead but has three alive neighbours, it is born in the next generation
                else if(value==0 && aliveNeighbours ==3){
                    Result.push_back(1);
                }
                // Cell is alive and has more than 3 neighbours or less than three then it dies
                else{

                    Result.push_back(0);
                }
                
            }

        }
        // all processes send their results and gather all of them into arr which will represent the next generation
        MPI_Allgather(&Result[0],offset*columns,MPI_INT,&arr[0],offset*columns,MPI_INT,MPI_COMM_WORLD);
        if(processID == 0){
            vector<vector<int>> b = convert1dT2D(arr,rows,columns);
            board = b ;
            PrintBoard(b,iteration,true,rows,columns);
        }
    }
    // auto end = chrono::high_resolution_clock::now();
    //  double timeTaken = chrono::duration_cast<chrono::duration<double>>(end - start).count();
    auto end = MPI_Wtime();
    auto timeTaken = end - start;
    MPI_Reduce(&timeTaken,&TotalTime,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if(processID == 0){
    ofstream File("ParallelResult.txt", ios_base::app);
    File<< "The total Serial time taken is: ";
    File<< SerialTime<<" seconds \n\n";
    File<< "The total Parallel time taken is: ";
    File<< TotalTime/NumProcesses <<" seconds \n\n";
    File<< "The total speedup is : ";
    File<< SerialTime/(TotalTime/NumProcesses) <<" seconds \n\n";
    }
    MPI_Finalize();
    return 0;
}