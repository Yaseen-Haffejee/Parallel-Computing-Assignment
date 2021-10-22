#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <mpi.h>

using namespace std;

int rows, columns, NumProcesses;
// please set the initial matrix here
// We just copy the matrix from matrix.txt which will be generated when we run BoardGenerator
vector<vector<int>> board = {
     {1, 0, 1, 1, 1, 1, 0, 0, 1, 1, },
{0, 1, 0, 1, 1, 0, 0, 0, 0, 0, },
{1, 0, 1, 1, 0, 0, 0, 1, 1, 1, },
{1, 0, 0, 0, 1, 1, 1, 0, 1, 0, },
{1, 1, 1, 1, 0, 1, 0, 0, 1, 0, },
{1, 0, 1, 0, 0, 1, 0, 0, 0, 1, },
{1, 1, 0, 1, 0, 1, 0, 1, 1, 1, },
{0, 1, 0, 1, 0, 1, 0, 0, 1, 0, },
{1, 0, 0, 0, 0, 0, 1, 1, 0, 1, },
{0, 0, 0, 0, 1, 0, 0, 0, 0, 1, },



    };
// Get the number of neighbours that are alive given a board and the position on the board
int NumberOfLiveNeighbours(vector<vector<int>>&board,int row,int col, int rows, int columns){
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
// Method that will print each generation to a file
void PrintBoard(vector<vector<int>>&board, int iteration){
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

int main(int argc, char * argv[]){
    // the number of processes being used, as well as the rows and columns of the matrix are passed in as arguments
    NumProcesses =  atoi(argv[1]);
    rows =  atoi(argv[2]);
    columns = atoi(argv[3]);
    // measuring the performance
    // auto start = chrono::high_resolution_clock::now();
    MPI_Init(&argc, &argv);
    // processID will identify each process and offset will determine how many rows of the matrix are being processed per process
    int processID,offset;
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);
    offset = columns/NumProcesses;
    // arr will be used to store the 1d representation of the matrix after the first generation since it is broadcasted
    vector<int> arr(rows*columns);
    // newBoard is where we store the resulting new state
    vector<vector<int>> newBoard(rows,vector<int>(columns));
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
    for(int iteration=0;iteration<5;iteration++){
        // at the bottom, since process 0 gathers all the data into the final answer, i update it's board there
        // for the others, they receive the broadcast message, which is store in arr. So we convert arr to 2d and update board
        if(iteration >= 1 && processID != 0){
            board = convert1dT2D(arr,rows,columns);
        }
        // The Result of our computation, i.e determining if cells are alive or dead for a processes respective rows are stored in Result
        int Result [offset*columns];
        int index = 0;
        for(int i = startPos; i <endPos; i++){
            for(int j = 0; j < columns; j++){
                int aliveNeighbours = NumberOfLiveNeighbours(board,i,j,rows,columns);
                // if the cell is alive with two or three neighbours, it remains alive
                if(board[i][j]==1 &&(aliveNeighbours ==2 || aliveNeighbours ==3)){
                    Result[index] = 1;
                }
                // if the cell is dead but has three alive neighbours, it is born in the next generation
                else if(board[i][j]==0 && aliveNeighbours ==3){
                    Result[index] = 1;
                }
                // Cell is alive and has more than 3 neighbours or less than three then it dies 
                else{

                    Result[index] = 0;
                }
                index ++;
            }
        }
        // We send Result, output of our computation, to process 0 that gathers them all and combines them
        if(processID != 0){
            MPI_Send(&Result,offset*columns,MPI_INT, 0,1,MPI_COMM_WORLD);
        }
        // loop and receive data from all the processes 
        for(int i=0; i<NumProcesses-1;i++){
            if(processID ==0 ){
                int incoming [offset*columns];
                MPI_Status status;
                MPI_Recv(&incoming,offset*columns,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&status);
                int start = status.MPI_SOURCE*offset;
                int k = 1;
                for(int i = 0;i<offset*columns;i+=columns){
                    vector<int> curr;
                    for(int j = i; j < columns*k; j++){
                        curr.push_back(incoming[j]);
                    }
                    newBoard[start] = curr;
                    start++;
                    k++;
                }
                
            }
        }
        // after we receive all the data from other processes, we also need to add what process 0 computed to the board
        if(processID == 0){
            int start = 0;
            int k = 1;
            for(int i = 0;i<offset*columns;i+=columns){
                vector<int> curr;
                for(int j = i; j < columns*k; j++){
                    curr.push_back(Result[j]);
                }
                newBoard[start] = curr;
                start++;
                k++;
            }
            // here is where we update board for process 0 as mentioned above
            board = newBoard;
            // print the result to a file called ParallelResults.txt
            PrintBoard(newBoard,iteration);
            // convert 2d result to 1d, and broadcast it since it will be the new board on the next iteration
            arr = convert2dTo1D(newBoard,rows,columns);
        }
        // actual broadcast message
        MPI_Bcast(&arr[0],rows*columns,MPI_INT,0,MPI_COMM_WORLD);
        
    }
    auto end = MPI_Wtime();
    auto timeTaken = end - start;
    ofstream File("ParallelResult.txt", ios_base::app);
    File<< "The total time taken is: ";
    File<< timeTaken <<" seconds \n\n";
    MPI_Finalize();
    // auto end = chrono::high_resolution_clock::now();
    // if(processID == 0){
    //     double timeTaken = chrono::duration_cast<chrono::duration<double>>(end - start).count();
    //     ofstream File("ParallelResult.txt", ios_base::app);
    //     File<< "The total time taken is: ";
    //     File<< timeTaken <<" seconds \n\n";
    // }


    return 0;
}