#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Result structure
typedef struct {
    double time;
    double convergenceTime;
    int distance;
    int *path;
    int iterations;
    int stagnationCount;
    double memoryUsage;
} Result;

// Utility functions
int **readDistanceMatrix(const char *filename, int *numCities);
void freeDistanceMatrix(int **matrix, int numCities);
double **allocateDoubleMatrix(int rows, int cols);
void freeDoubleMatrix(double **matrix, int rows);
double getCurrentTime();
int calculatePathDistance(const int *path, int **distanceMatrix, int numCities);
double calculateMemoryUsage(int numCities, int additionalAllocations);
int *generateRandomPath(int numCities);

// Stability testing
void runStabilityTests(int **distanceMatrix, int numCities);
void printStabilityResults(double gaAvg, double gaVar, double acoAvg, double acoVar);

#endif // UTILITY_H
