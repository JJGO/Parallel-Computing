#!/bin/sh
#PBS -S /bin/sh
#PBS -N hw3
#PBS -A eecs587f15_flux
#PBS -l qos=flux
#PBS -l procs=36,walltime=0:05:00
#PBS -l pmem=1024mb
#PBS -q flux
#PBS -m abe
#PBS -j oe
#PBS -V
echo "I ran on:"
cat $PBS_NODEFILE
# Let PBS handle your output
cd /home/jjgo/Documents/Homework3
mpirun -np 36 ./matrix 1000
mpirun -np 36 ./matrix 1000
mpirun -np 36 ./matrix 1000
mpirun -np 36 ./matrix 2000
mpirun -np 36 ./matrix 2000
mpirun -np 36 ./matrix 2000
