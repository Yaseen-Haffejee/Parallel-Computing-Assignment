#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <assert.h>
#include "Conway.h"
#include <mpi.h>

using namespace std;

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
    double SerialTime = 0.0;
    // SerialResults holds 1-D representations of the board after a serial iteration.
    // Example: If we run 5 iterations, SerialResults will hold 5 board representations in 1-D format, each representation is the resulting board(new generation) after an iteration
    vector<vector<int>>SerialResults;
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);
    if(processID ==0 ){
        // we let process 0 run the serial part and we time it
        auto startSerial = chrono::high_resolution_clock::now();
        // SerialConway returns the results in the vector forms, which we store in SerialResults as explained above
        SerialResults = SerialConway(argc,argv,Board);
        auto endSerial = chrono::high_resolution_clock::now();
        // Store how long it took to complete the entire serial calculation
        SerialTime = chrono::duration_cast<chrono::duration<double>>(endSerial - startSerial).count();
    }
    // We place a barrier here, since we want all the processes to execute at the same time from the same point
    // All the processes wait for processes 0 to complete the serial implementation before beginning the parallel one
    MPI_Barrier(MPI_COMM_WORLD);
    // Start timing the parallel implementation
    auto start = MPI_Wtime();
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
            // convert arr into 2d for process 0
            vector<vector<int>> b = convert1dT2D(arr,rows,columns);
            // check that the SerialResult for the generation is the same as the parallel result
            if(SerialResults[iteration] == arr){
                printf("After iteration %d , the Serial and Parallel Results are Equal\n\n",iteration);
            }
            else{
                printf("After iteration %d , the Serial and Parallel Results are NOT Equal\n\n",iteration);
            }
            board = b ;
            // If you would like to see each generation, uncomment the line below and the results are printed to a file called ParallelResult.txt
            // PrintBoard(b,iteration,true,rows,columns);
        }
    }
    // Stop the timing
    auto end = MPI_Wtime();
    auto timeTaken = end - start;
    if(processID == 0){
        // Serial Time and Parallel time is the average so we Divide it by iters which stores the number of iterations we did
        ofstream File("Results.txt", ios_base::app);
        File<<"Rows = "<< rows<< " Columns = "<< columns<<" Iterations = "<<iters<<" Processes = "<<NumProcesses<<endl;
        cout<<"Rows = "<< rows<< " Columns = "<< columns<<" Iterations = "<<iters<<" Processes = "<<NumProcesses<<endl;
        File<<"\n";
        cout<<"\n";
        File<< "The total Serial time taken is: ";
        cout<< "The total Serial time taken is: ";
        File<< SerialTime/iters<<" seconds \n\n";
        cout<< SerialTime/iters<<" seconds \n\n";
        File<< "The total Parallel time taken is: ";
        cout<< "The total Parallel time taken is: ";
        File<< (timeTaken)/iters <<" seconds \n\n";
        cout<< (timeTaken)/iters <<" seconds \n\n";
        File<< "The total speedup is : ";
        cout<< "The total speedup is : ";
        // Speed up is Serial/ Parallel
        File<< (SerialTime)/(timeTaken) <<" seconds \n\n";
        File<<"---------------------------------------------------------------\n\n";
        cout<< (SerialTime)/(timeTaken) <<" seconds \n\n";
    }
    MPI_Finalize();
    return 0;
}