#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
using namespace std;
int rows;
int columns ;

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
// Serial Implementation of Conway's game of life
void SerialConway(vector<vector<int>>&board){
    vector<vector<int>> nextGen(rows,vector<int>(columns));
    // We have to loop through the entire board and check the neighbours and adjust whether a cell is alive or dead
    for(int i =0; i<rows; i++){
        for(int j =0; j<columns;j++){
            int aliveNeighbours = NumberOfLiveNeighbours(board,i,j,rows,columns);
            // if the cell is alive with two or three neighbours, it remains alive
            if(board[i][j]==1 &&(aliveNeighbours ==2 || aliveNeighbours ==3)){
                nextGen[i][j] = 1;
            }
            // if the cell is dead but has three alive neighbours, it is born in the next generation
            else if(board[i][j]==0 && aliveNeighbours ==3){
                nextGen[i][j] = 1;
            }
            // Cell is alive and has more than 3 neighbours or less than three then it dies 
            else{
                nextGen[i][j] = 0;
            }
        }
    }
    // update board to be the next generation
    board = nextGen;
}

void PrintBoard(vector<vector<int>>&board, int iteration){
    ofstream Matrix("SerialResult.txt", ios_base::app);
    Matrix << "The serial Result after iteration " <<iteration << " is: \n";
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


int main(int argc, char * argv[]){
    // rows and columns are global integers defined right at the top
    rows = stoi(argv[1]);
    columns = stoi(argv[2]);
    vector<vector<int> >board = {
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
    auto start = chrono::high_resolution_clock::now();
    for(int i = 0; i <5;i++){
        SerialConway(board);
        PrintBoard(board,i);
    }
    auto end = chrono::high_resolution_clock::now();
    double timeTaken = chrono::duration_cast<chrono::duration<double>>(end - start).count();
    ofstream File("SerialResult.txt", ios_base::app);
    File<< "The total time taken is: ";
    File<< timeTaken <<" seconds \n\n";

    return 0;
}
