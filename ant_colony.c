#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include<limits.h>
#include "ant_colony.h"
#include "utility.h"

#define ALPHA 1.0
#define BETA 2.0
#define RHO 0.5
#define Q 100

Result runAntColonyOptimization(int **distanceMatrix, int numCities) {
    int numAnts = numCities;  // One ant per city
    int maxIterations = 100;

    // Initialize pheromone matrix
    double **pheromone = allocateDoubleMatrix(numCities, numCities);
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            pheromone[i][j] = 1.0;  // Initial pheromone value
        }
    }

    int *bestPath = (int *)malloc(numCities * sizeof(int));
    int bestDistance = INT_MAX;
    double startTime = getCurrentTime();
    double convergenceTime = 0.0;

    for (int iter = 0; iter < maxIterations; iter++) {
        int **paths = (int **)malloc(numAnts * sizeof(int *));
        for (int k = 0; k < numAnts; k++) {
            paths[k] = generateRandomPath(numCities);
        }

        // Evaluate paths
        for (int k = 0; k < numAnts; k++) {
            int distance = calculatePathDistance(paths[k], distanceMatrix, numCities);
            if (distance < bestDistance) {
                bestDistance = distance;
                memcpy(bestPath, paths[k], numCities * sizeof(int));
                convergenceTime = getCurrentTime() - startTime;
            }
        }

        // Update pheromone
        for (int i = 0; i < numCities; i++) {
            for (int j = 0; j < numCities; j++) {
                pheromone[i][j] *= (1.0 - RHO);  // Evaporation
            }
        }
        for (int k = 0; k < numAnts; k++) {
            int distance = calculatePathDistance(paths[k], distanceMatrix, numCities);
            for (int i = 0; i < numCities - 1; i++) {
                int from = paths[k][i];
                int to = paths[k][i + 1];
                pheromone[from][to] += Q / (double)distance;
                pheromone[to][from] += Q / (double)distance;  // Undirected graph
            }
        }

        for (int k = 0; k < numAnts; k++) {
            free(paths[k]);
        }
        free(paths);
    }

    double endTime = getCurrentTime();
    freeDoubleMatrix(pheromone, numCities);

    Result result = {
        .time = endTime - startTime,
        .convergenceTime = convergenceTime,
        .distance = bestDistance,
        .path = bestPath,
        .iterations = maxIterations,
        .stagnationCount = 0,
        .memoryUsage = calculateMemoryUsage(numCities, numAnts)
    };
    return result;
}
