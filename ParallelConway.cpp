#include <vector>
#include <iostream>
#include <fstream>
#include <mpi.h>

using namespace std;

int rows, columns, NumProcesses;
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
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, }


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
int main(int argc, char * argv[]){
    NumProcesses =  atoi(argv[1]);
    rows =  atoi(argv[2]);
    columns = atoi(argv[3]);
    vector<vector<int>> newBoard(rows,vector<int>(columns));
    int next,offset;
    MPI_Init(&argc, &argv);
    int processID;
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);
    offset = columns/NumProcesses;
    if(processID == NumProcesses -1 ){
        next = 0;
    }
    else{
        next = processID + 1;
    }
   
    vector<vector<int>>Result;
    vector<vector<int>>b = board;
    int startPos = 0;
    int endPos = offset;
    if(processID != 0){
        startPos = processID * offset;
        endPos = startPos + offset;
    }
    for(int i = startPos; i <endPos; i++){
        vector<int>nextGen(columns);
        for(int j = 0; j < columns; j++){
            int aliveNeighbours = NumberOfLiveNeighbours(b,i,j,rows,columns);
            // if the cell is alive with two or three neighbours, it remains alive
            if(b[i][j]==1 &&(aliveNeighbours ==2 || aliveNeighbours ==3)){
                nextGen[j] = 1;
            }
            // if the cell is dead but has three alive neighbours, it is born in the next generation
            else if(b[i][j]==0 && aliveNeighbours ==3){
                nextGen[j] = 1;
            }
            // Cell is alive and has more than 3 neighbours or less than three then it dies 
            else{

                nextGen[j] = 0;
            }
            
        }
        Result.push_back(nextGen);
        Print(nextGen);
    }

    if(processID != 0){
        // MPI_Send(&(Result[0][0]),offset*columns,MPI_INT, 0,1,MPI_COMM_WORLD);
        MPI_Send(&(Result[0][0]),offset*columns,MPI_INT, 0,1,MPI_COMM_WORLD);
        // printf("processID = %d\n",processID);
    }
    // printf("Fcuk %d is here\n",processID);
    
    for(int i=0; i<NumProcesses-1;i++){
        if(processID ==0 ){
            vector<vector<int>> incoming(offset,vector<int>(columns));
            MPI_Status status;
            MPI_Recv(&(incoming[0][0]),offset*columns,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&status);
            // printf("the incoming message is from %d\n",status.MPI_SOURCE);
            // Print(incoming);
            // printf("from %d size is %d\n",status.MPI_SOURCE,incoming.size());
            int start = status.MPI_SOURCE*offset;
            for(int i = 0;i<offset;i++){
                newBoard[start + i] = incoming[i];
            }
            
        }
    }
    if(processID == 0){
        for(int i=0;i<offset;i++){
            newBoard[i] = Result[i];
        }
        PrintBoard(newBoard,0);
    }
    


    MPI_Finalize();


    return 0;
}