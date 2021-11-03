
rows = 5000
columns = 5000
Processes =  4
iterations = 5

All: Conway
	mpiexec -n ${Processes} ./final ${Processes} ${rows} ${columns} ${iterations}

Conway: BoardGenerator.o Serialconway.o Parallelconway.o
	mpic++ BoardGenerator.o SerialConway.o ParallelConway.o -o final

BoardGenerator: BoardGenerator.o
	./BoardGenerator ${rows} ${columns}

BoardGenerator.o: BoardGenerator.cpp Conway.h
	g++ -c BoardGenerator.cpp 

Serialconway : Serialconway.o
	./Serialconway ${rows} ${columns} ${iterations}

Serialconway.o : SerialConway.cpp Conway.h
	g++ -c SerialConway.cpp

Parallelconway : Parallelconway.o
	mpiexec -n ${Processes} ./Parallelconway ${Processes} ${rows} ${columns} ${iterations}

Parallelconway.o : ParallelConway.cpp Conway.h
	mpic++ -c ParallelConway.cpp

clean: 
	rm -f *conway *.txt BoardGenerator *.o final
