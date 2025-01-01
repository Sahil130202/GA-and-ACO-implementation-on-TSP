all: tsp_project

tsp_project: main.c genetic.c ant_colony.c utility.c
	gcc -o tsp_project main.c genetic.c ant_colony.c utility.c -lm

clean:
	rm -f tsp_project
