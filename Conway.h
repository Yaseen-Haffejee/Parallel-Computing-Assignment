#ifndef Conway_h
# define Conway_h
#include <vector>

using namespace std;


int ParallelConway(int argc, char * argv[],vector<vector<int>>Board);
double SerialConway(int argc, char * argv[],vector<vector<int>>Board);
void PrintBoard(vector<vector<int>>&board, int rows,int columns);
vector<vector<int>> GenerateBoard(int rows, int columns);
int NumberOfLiveNeighbours(vector<vector<int>>&board,int row,int col, int rows, int columns);
#endif