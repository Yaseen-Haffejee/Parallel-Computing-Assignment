vertices = 3200
Processes = 4
source = 0

All: GraphGenerator OpenMPDijkstra MPIDijkstra


GraphGenerator:	GraphGeneratorBuild
	./GraphGenerator ${vertices}

GraphGeneratorBuild:	GraphGenerator.cpp
	g++ -o GraphGenerator GraphGenerator.cpp

OpenMPDijkstra: openmp
	./omp ${vertices} ${source} ${Processes}

openmp: SerialDijkstra.cpp Dijkstra.h OpenMPDijkstra.cpp
	g++ -o omp SerialDijkstra.cpp -fopenmp OpenMPDijkstra.cpp


MPIDijkstra: mpi
	mpiexec -n ${Processes} ./MPIDijkstra ${vertices} ${source} ${Processes}

mpi: SerialDijkstra.cpp Dijkstra.h MPIDijkstra.cpp
	mpic++ -o MPIDijkstra SerialDijkstra.cpp MPIDijkstra.cpp

clean: 

	rm -f *Dijkstra GraphGenerator *.txt *.o omp