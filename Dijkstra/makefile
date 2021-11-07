vertices = 512
Processes = 4
source = 0


all: GraphGenerator MPIDijkstra

GraphGenerator:	GraphGeneratorBuild
	./GraphGenerator ${vertices}

GraphGeneratorBuild:	GraphGenerator.cpp
	g++ -o GraphGenerator GraphGenerator.cpp

MPIDijkstra: mpi
	mpiexec -n ${Processes} ./MPIDijkstra ${vertices} ${source} ${Processes}

mpi: SerialDijkstra.cpp OpenMPDijkstra.cpp Dijkstra.h MPIDijkstra.cpp
	mpic++ -o MPIDijkstra SerialDijkstra.cpp -fopenmp OpenMPDijkstra.cpp MPIDijkstra.cpp

clean: 

	rm -f *Dijkstra GraphGenerator *.txt *.o omp