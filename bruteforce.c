#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

struct city {
    int x;
    int y;
}; typedef struct city city;

double distancecalculator(int* array, double** distances, int numcities) { //instead of the lots of pythagorean theorems I was using in the main code, in this i noticed I could calculate the distances only once and store them in a matrix.
    double resp = 0;
    for(int i = 0; i < numcities; i++) {
        if(i == numcities-1) {
            resp += distances[array[i]][array[0]];
            break;
        }
        resp += distances[array[i]][array[i+1]];
    }
    return resp;
}
int next_permutation(int *arr, int *end) {
    if (end - arr <= 1)
        return 0;

    int *last = end - 1;

    while (1) {
        int *second_last = last--;

        if (*last < *second_last) {
            int *next = end;
            while (*--next <= *last)
                ;

            int tmp = *last;
            *last = *next;
            *next = tmp;

            for (; ++last < --end;) {
                tmp = *last;
                *last = *end;
                *end = tmp;
            }
            return 1;
        }

        if (last == arr) {
            for (; ++last < --end;) {
                int tmp = *last;
                *last = *end;
                *end = tmp;
            }
            return 0;
        }
    }
}

int main(void) {

    clock_t starttime = clock();//stores the initial clock 
    FILE* file = fopen("cities.bin", "rb"); // gets the cities generated by the cidades.c program
    if(file == NULL) {
        printf("Error in opening file\n");
        return 1;
    } 

    int numcities;
    fread(&numcities, sizeof(int), 1, file);

    struct individual { //I know its hard to endure a struct being declared inside the main function, but this was the easiest way I found to be able to read the numofcities from the file without unecessary dynamic allocation
        int array[numcities];
        double fitness;
    }; typedef struct individual individual;

    city cities[numcities];

    fread(cities, sizeof(city), numcities, file);//reads the array of cities

    double** distances = (double**) malloc(sizeof(double*) * numcities);//create the matrix for storing the distances without needing to calculate them every time
    for(int i = 0; i < numcities; i++) {
        distances[i] = (double*) malloc(sizeof(double) * numcities);
    }
    for(int i = 0; i < numcities; i++) {
        for(int j = 0; j < numcities; j++) {
            distances[i][j] = sqrt(pow((cities[i].x - cities[j].x), 2) + pow((cities[i].y - cities[j].y), 2));
        }
    }

    individual best;
    for(int i = 0; i < numcities; i++) {
        best.array[i] = i;
    }
    best.fitness = distancecalculator(best.array, distances, numcities);

    // Brute-force algorithm
    int temp[numcities];
    for(int i = 0; i < numcities; i++) {
        temp[i] = i;
    }

    do {
        double currentFitness = distancecalculator(temp, distances, numcities);
        if (currentFitness < best.fitness) {
            best.fitness = currentFitness;
            for (int i = 0; i < numcities; i++) {
                best.array[i] = temp[i];
            }
        }
    } while (next_permutation(temp, temp + numcities));

    // Print the best solution found
    printf("Best solution: ");
    for (int i = 0; i < numcities; i++) {
        printf("%d ", best.array[i]);
    }
    clock_t finaltime = clock();//gets the final time 
    double timetaken = (double)(finaltime - starttime)/CLOCKS_PER_SEC;//gets that time in seconds
    printf("\nTotal distance: %f Total time: %lf seconds\n", best.fitness, timetaken);
    FILE* file2 = fopen("solved.bin", "wb");
    fwrite(&best, sizeof(individual), 1, file2);
    fwrite(&timetaken, sizeof(double), 1, file2);
    fclose(file2);
    fclose(file);
    return 0;
}