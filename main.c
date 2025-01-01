#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "genetic.h"
#include "ant_colony.h"

int main() {
    // Read the TSP input file
    int numCities;
    int **distanceMatrix = readDistanceMatrix("tsp_input.txt", &numCities);

    if (!distanceMatrix) {
        return 1; // Exit if the input file couldn't be read
    }

    // Run Genetic Algorithm
    printf("Running Genetic Algorithm...\n");
    Result gaResult = runGeneticAlgorithm(distanceMatrix, numCities);
    printf("GA Completed:\n");
    printf("    Time: %.2f ms\n", gaResult.time);
    printf("    Distance: %d\n", gaResult.distance);
    printf("    Path: ");
    for (int i = 0; i < numCities; i++) {
        printf("%d -> ", gaResult.path[i]);
    }
    printf("%d\n", gaResult.path[0]);
    printf("    Iterations to Converge: %d\n", gaResult.iterations);
    printf("    Memory Usage: %.2f KB\n\n", gaResult.memoryUsage);

    // Run Ant Colony Optimization
    printf("Running Ant Colony Optimization...\n");
    Result acoResult = runAntColonyOptimization(distanceMatrix, numCities);
    printf("ACO Completed:\n");
    printf("    Time: %.2f ms\n", acoResult.time);
    printf("    Distance: %d\n", acoResult.distance);
    printf("    Path: ");
    for (int i = 0; i < numCities; i++) {
        printf("%d -> ", acoResult.path[i]);
    }
    printf("%d\n", acoResult.path[0]);
    printf("    Iterations to Converge: %d\n", acoResult.iterations);
    printf("    Memory Usage: %.2f KB\n\n", acoResult.memoryUsage);

    // Run stability tests
    printf("Running Stability Tests...\n");
    runStabilityTests(distanceMatrix, numCities);

    // Free allocated resources
    free(gaResult.path);
    free(acoResult.path);
    freeDistanceMatrix(distanceMatrix, numCities);

    return 0;
}
