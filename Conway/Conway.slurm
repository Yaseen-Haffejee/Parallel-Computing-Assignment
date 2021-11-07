#!/bin/bash
# specify a partition
#SBATCH -p batch
# specify number of nodes
#SBATCH -N 4
# specify number of tasks
#SBATCH --ntasks=8
# specify number of cores (or threads) per process
##SBATCH -c 2
# specify memory pool per node (for all cores on the node)
##SBATCH --mem 10
# specify the wall clock time limit for the job (hh:mm:ss) 5 mins
##SBATCH -t 5:00
# specify the job name
#SBATCH -J PC_Assignment_Conway
# specify the filename to be used for writing output 
# change the path for your own account here
#SBATCH -o /home-mscluster/yhaffejee/Assignment/Conway/output/%N.%j.out
# specify the filename for stderr
# change the path for your own account here
#SBATCH -e /home-mscluster/yhaffejee/Assignment/Conway/error/%N.%j.err

make clean

make All
