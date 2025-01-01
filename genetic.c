#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<limits.h>
#include "genetic.h"
#include "utility.h"

// Genetic Algorithm Implementation
Result runGeneticAlgorithm(int **distanceMatrix, int numCities) {
    srand(time(NULL));
    int populationSize = 50;
    int generations = 100;
    double mutationRate = 0.1;

    int **population = (int **)malloc(populationSize * sizeof(int *));
    for (int i = 0; i < populationSize; i++) {
        population[i] = generateRandomPath(numCities);
    }

    int *bestPath = (int *)malloc(numCities * sizeof(int));
    int bestDistance = INT_MAX;
    double startTime = getCurrentTime();
    double convergenceTime = 0.0;

    for (int gen = 0; gen < generations; gen++) {
        for (int i = 0; i < populationSize; i++) {
            int distance = calculatePathDistance(population[i], distanceMatrix, numCities);
            if (distance < bestDistance) {
                bestDistance = distance;
                convergenceTime = getCurrentTime() - startTime;
                memcpy(bestPath, population[i], numCities * sizeof(int));
            }
        }

        // Mutation
        for (int i = 0; i < populationSize; i++) {
            if ((rand() / (double)RAND_MAX) < mutationRate) {
                int idx1 = rand() % numCities;
                int idx2 = rand() % numCities;
                int temp = population[i][idx1];
                population[i][idx1] = population[i][idx2];
                population[i][idx2] = temp;
            }
        }
    }

    double endTime = getCurrentTime();
    for (int i = 0; i < populationSize; i++) {
        free(population[i]);
    }
    free(population);

    Result result = {
        .time = endTime - startTime,
        .convergenceTime = convergenceTime,
        .distance = bestDistance,
        .path = bestPath,
        .iterations = generations,
        .stagnationCount = 0,
        .memoryUsage = calculateMemoryUsage(numCities, populationSize)
    };
    return result;
}
