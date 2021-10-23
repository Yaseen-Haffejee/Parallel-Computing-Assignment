
rows = 1000
columns = 10
Processes =  8
iterations = 5

BoardGenerator: BoardGenerator.o
	./BoardGenerator ${rows} ${columns}

BoardGenerator.o: BoardGenerator.cpp
	g++ BoardGenerator.cpp -o BoardGenerator

Serialconway : Serialconway.o
	./Serialconway ${rows} ${columns} ${iterations}


Serialconway.o : SerialConway.cpp
	g++ SerialConway.cpp -o Serialconway

Parallelconway : Parallelconway.o
	mpiexec -n ${Processes} ./Parallelconway ${Processes} ${rows} ${columns} ${iterations}


Parallelconway.o : ParallelConway.cpp
	mpic++ ParallelConway.cpp -o Parallelconway

clean: 
	rm *conway *.txt BoardGenerator