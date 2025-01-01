#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "utility.h"
#include "genetic.h"
#include "ant_colony.h"

// Read the distance matrix from a file
int **readDistanceMatrix(const char *filename, int *numCities) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return NULL;
    }

    fscanf(file, "%d", numCities);
    int **matrix = (int **)malloc((*numCities) * sizeof(int *));
    for (int i = 0; i < *numCities; i++) {
        matrix[i] = (int *)malloc((*numCities) * sizeof(int));
        for (int j = 0; j < *numCities; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
    fclose(file);
    return matrix;
}

// Free the distance matrix
void freeDistanceMatrix(int **matrix, int numCities) {
    for (int i = 0; i < numCities; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Allocate memory for a double matrix
double **allocateDoubleMatrix(int rows, int cols) {
    double **matrix = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double *)malloc(cols * sizeof(double));
    }
    return matrix;
}

// Free the double matrix
void freeDoubleMatrix(double **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Get the current time in milliseconds
double getCurrentTime() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000.0) + (time.tv_usec / 1000.0);
}

// Calculate the total distance of a given path
int calculatePathDistance(const int *path, int **distanceMatrix, int numCities) {
    int distance = 0;
    for (int i = 0; i < numCities - 1; i++) {
        distance += distanceMatrix[path[i]][path[i + 1]];
    }
    distance += distanceMatrix[path[numCities - 1]][path[0]];  // Return to start
    return distance;
}

// Generate a random TSP path
int *generateRandomPath(int numCities) {
    int *path = (int *)malloc(numCities * sizeof(int));
    for (int i = 0; i < numCities; i++) {
        path[i] = i;
    }
    for (int i = numCities - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = path[i];
        path[i] = path[j];
        path[j] = temp;
    }
    return path;
}

// Calculate memory usage
double calculateMemoryUsage(int numCities, int additionalAllocations) {
    return (numCities * numCities * sizeof(int)) + (additionalAllocations * sizeof(int *));
}

// Run stability tests and print results
void runStabilityTests(int **distanceMatrix, int numCities) {
    double gaTimes[5], acoTimes[5];
    double gaSum = 0, acoSum = 0;

    for (int i = 0; i < 5; i++) {
        Result gaResult = runGeneticAlgorithm(distanceMatrix, numCities);
        Result acoResult = runAntColonyOptimization(distanceMatrix, numCities);

        gaTimes[i] = gaResult.time;
        acoTimes[i] = acoResult.time;

        gaSum += gaResult.time;
        acoSum += acoResult.time;

        free(gaResult.path);
        free(acoResult.path);
    }

    double gaAvg = gaSum / 5.0;
    double acoAvg = acoSum / 5.0;

    double gaVariance = 0, acoVariance = 0;
    for (int i = 0; i < 5; i++) {
        gaVariance += (gaTimes[i] - gaAvg) * (gaTimes[i] - gaAvg);
        acoVariance += (acoTimes[i] - acoAvg) * (acoTimes[i] - acoAvg);
    }
    gaVariance /= 5.0;
    acoVariance /= 5.0;

    printStabilityResults(gaAvg, gaVariance, acoAvg, acoVariance);
}

// Print stability results
void printStabilityResults(double gaAvg, double gaVar, double acoAvg, double acoVar) {
    printf("Stability Results:\n");
    printf("Genetic Algorithm: Avg Time: %.2f ms, Variance: %.2f\n", gaAvg, gaVar);
    printf("Ant Colony Optimization: Avg Time: %.2f ms, Variance: %.2f\n", acoAvg, acoVar);
}
