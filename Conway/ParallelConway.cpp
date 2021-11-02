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
int main(int argc, char * argv[]){
    // the number of processes being used, as well as the rows and columns of the matrix are passed in as arguments
    int NumProcesses =  atoi(argv[1]);
    int rows =  atoi(argv[2]);
    int columns = atoi(argv[3]);
    int iters = atoi(argv[4]);
    vector<vector<int> > Board = GenerateBoard(rows,columns);
    // measuring the performance
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
    if(processID == 0){
        if(rows%NumProcesses != 0){
            cout<< "Please ensure that rows/processes(divide) is an integer and not a float.\n";
            exit(0);
        }
    }
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
                int aliveNeighbours = NumberOfLiveNeighbours(board,i,j,rows,columns);
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
        cout<<"Rows = "<< rows<< " Columns = "<< columns<<" Iterations = "<<iters<<" Processes = "<<NumProcesses<<endl;
        cout<<"\n";
        cout<< "The total Serial time taken is: ";
        cout<< SerialTime/iters<<" seconds \n\n";
        cout<< "The total Parallel time taken is: ";
        cout<< (timeTaken)/iters <<" seconds \n\n";
        cout<< "The total speedup is : ";
        // Speed up is Serial/ Parallel
        cout<< (SerialTime)/(timeTaken) <<" seconds \n\n";
    }
    MPI_Finalize();
    return 0;
}