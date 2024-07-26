all: main.cpp
	g++ main.cpp -o main

run1A:
	srun ./main '1A' '/common/contrib/classroom/inf503/human_reads_version2.fa' '/common/contrib/classroom/inf503/genomes/human.txt'

run1B:
	srun ./main '1B' '/common/contrib/classroom/inf503/human_reads_version2.fa' '/common/contrib/classroom/inf503/genomes/human.txt'

run2A:
	srun ./main '2A' '/common/contrib/classroom/inf503/human_reads_version2.fa' '/common/contrib/classroom/inf503/genomes/human.txt'

run2B:
	srun ./main '2B' '/common/contrib/classroom/inf503/human_reads_version2.fa' '/common/contrib/classroom/inf503/genomes/human.txt'

clean:
	rm -f main *.o