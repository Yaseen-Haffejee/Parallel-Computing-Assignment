#include "Conway.h"
#include <string>
#include "mpi.h"
using namespace std;

int main(int argc, char * argv[]){
    int NumProcesses =  atoi(argv[1]);
    int rows =  atoi(argv[2]);
    int columns = atoi(argv[3]);
    int iters = atoi(argv[4]);
    int rank;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    vector<vector<int> > Board = GenerateBoard(rows,columns);
    // PrintBoard(Board,rows,columns);
    if(rank ==0 ){
        SerialConway(argc,argv,Board);
        
    }
    MPI_Finalize();
}