all: main.cpp
	g++ main.cpp -o main

runA:
	srun ./main 'A' '/common/contrib/classroom/inf503/human_reads_trimmed.fa' '/common/contrib/classroom/inf503/genomes/human.txt'

runB:
	srun ./main 'B' '/common/contrib/classroom/inf503/human_reads_trimmed.fa' '/common/contrib/classroom/inf503/genomes/human.txt'

clean:
	rm -f main *.o