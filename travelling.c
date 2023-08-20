#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define numcities 15

struct city {
    int x;
    int y;
}; typedef struct city city;

struct individual {
    int array[numcities];
    double fitness;
}; typedef struct individual individual;

//function prototypes
void shuffle(int arr[], int length);
void score(int array[numcities], double* fitness, city cities[numcities]);

//functions
void shuffle(int arr[], int length) {
    for (int i = length - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void score(int array[numcities], double* fitness, city cities[numcities]) {

    double distance = 0;   //each individual fitness is based on the distances between two cities following the order in its own array
                            //sum of all the distances between one city and the next

    for(int i = 0; i < numcities; i++) {
        if(i == numcities - 1) {
            distance += sqrt(pow(cities[array[i]].x - cities[array[0]].x, 2) + pow(cities[array[i]].y - cities[array[0]].y, 2));
            break;
        }
        distance += sqrt(pow(cities[array[i]].x - cities[array[i+1]].x, 2) + pow(cities[array[i]].y - cities[array[i+1]].y, 2));
    }
    *fitness = distance;
}


int main(int argc, char* argv[]) {

    if(argc != 3) {
        printf("The correct use of the program is './travelling num_sizeofpopulation num_howmanygenerations'\n");
        return 1;
    }

    int sizeofpop = atoi(argv[1]);
    int numgenerations = atoi(argv[2]);
    int mutation = 1;
    time_t t;
    srand((unsigned) time(&t));

    FILE* file = fopen("cities.bin", "rb");
    if(file == NULL) {
        printf("Error while opening file\n");
        return 1;
    }

    city cities[numcities];
    individual population[sizeofpop];

    fread(cities, sizeof(city), numcities, file);

    //initializing
    for(int i = 0; i < sizeofpop; i++) {
        for(int j = 0; j < numcities; j++) {
            population[i].array[j] = j; //initialize each individual with the sequence {0, 1, 2, ...}
        }
    }

    for(int i = 0; i < sizeofpop; i++) {
        shuffle(population[i].array, numcities); //part 2 of initializing is shuffling them all
    }

    score(population[0].array, &population[0].fitness, cities); //initialize the variable best
    individual best = population[0];
    int generation = 1;
    int repeatedbest = 0;
    individual previousbest;

    while(generation <= numgenerations) {

        //selection and scoring
        for(int i = 0; i < sizeofpop; i++) {
            score(population[i].array, &population[i].fitness, cities);
            if(population[i].fitness < best.fitness) {
                best = population[i]; //the best is the one with the lowest distance
            }
        }
    
        //mutation
        
        if(previousbest.fitness == best.fitness) {
                repeatedbest++;
        } else {
            repeatedbest = 0;
            mutation = 1;
        }
        if(repeatedbest >= 2500) {
            if(mutation < numcities)
                mutation++;
            else    
                mutation = 1; //to not let it grow unnecessary bigger and keep it in a cycle
            repeatedbest = 0;
        }
        for(int i = 0; i < sizeofpop; i++) {
            for(int j = 0; j < mutation; j++) {
                int index1 = rand() % numcities, index2 = rand() % numcities;
                int temp = population[i].array[index1];
                population[i].array[index1] = population[i].array[index2];  //simple random change between two of the cities in the array, happens as many times as the number in mutation
                population[i].array[index2] = temp;
            }
        }
        population[0] = best; //I can't lose the best
        previousbest = best; //to keep track of how many times it was the same
        printf("%d %d %lf\n", generation, mutation, best.fitness);
        generation++;
    }

    fclose(file);
    return 0;
}