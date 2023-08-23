#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


struct city {//the city struct
    int x;
    int y;
}; typedef struct city city;

//function prototypes
void shuffle(int arr[], int length);
void score(int array[], double* fitness, city cities[], int numcities);
void circle(SDL_Renderer* renderer, int centerX, int centerY, int radius);

//functions

void circle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14 / 180.0;
        int x = centerX + radius * cos(angle);  //does basic trygonometry calculations to print 361 dots to create a circle 
        int y = centerY + radius * sin(angle);
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

void shuffle(int arr[], int length) {   //does the Fisher–Yates shuffle
    for (int i = length - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void score(int array[], double* fitness, city cities[],int numcities) {

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
    }//testing if the .exe was called correctly

    int quit = 0; //the variable necessary to make the while loop stop when I want to stop the program
    //sdl stuff, just initializing the sdl variables
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Event event;
    SDL_Window* window = SDL_CreateWindow("Travelling Salesman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 620, 620, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("font.ttf", 20);
    SDL_Color textColor = {0, 0, 0, 255};
    //still sdl stuff
    int sizeofpop = atoi(argv[1]);
    int numgenerations = atoi(argv[2]);
    int mutation = 1;
    time_t t;
    clock_t initialtime = clock();
    srand((unsigned) time(&t));

    FILE* file = fopen("cities.bin", "rb");
    FILE* file2 = fopen("solved.bin", "rb");
    if(file == NULL || file2 == NULL) {
        printf("Error while opening file\n");
        return 1;
    }

    int numcities;
    fread(&numcities, sizeof(int), 1, file);

    struct individual {
        int array[numcities];
        double fitness;
    }; typedef struct individual individual;

    individual solved;
    double bruteforcetime;
    fread(&solved, sizeof(individual), 1, file2);
    fread(&bruteforcetime, sizeof(double), 1, file2);

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

    score(population[0].array, &population[0].fitness, cities, numcities); //initialize the variable best
    individual best;
    best = population[0];
    int generation = 1;
    int repeatedbest = 0;
    individual previousbest;

    while(((generation <= numgenerations)||(numgenerations == -1)) && (quit == 0) && (best.fitness - solved.fitness > 0.0001)) {
        while (SDL_PollEvent(&event)) {//this reads all the events that are stored in the events array (this array is emptied after it is read)
            if (event.type == SDL_QUIT) {
                quit = 1; // this quit thing means that if the array of events detect a single sdl_quit, which is a ctrl c or a click on the x button, it will automatically change quit to 1 and thus making the while loop false
            }
        }
        //selection and scoring
        for(int i = 0; i < sizeofpop; i++) {
            score(population[i].array, &population[i].fitness, cities, numcities);
            if(population[i].fitness < best.fitness) {
                best = population[i]; //the best is the one with the lowest distance
            }
        }
    
        //mutation
        
        if(previousbest.fitness == best.fitness) {
                repeatedbest++;
        } else {
            repeatedbest = 0;//if a new best is found the mutation should go back to its original value as it may now try to do slight improvements.
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
            population[i] = best; //all the population is now a clone of the best
            for(int j = 0; j < mutation; j++) {
                int index1 = rand() % numcities, index2 = rand() % numcities;
                int temp = population[i].array[index1];
                population[i].array[index1] = population[i].array[index2];  //simple random change between two of the cities in the array, happens as many times as the number in mutation
                population[i].array[index2] = temp;
            }
        }
        previousbest = best; //to keep track of how many times it was the same
        printf("%d %lf\n", generation, best.fitness);
        generation++;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            for (int i = 0; i < numcities; i++) {//loop to draw the lines in both tracks (the best and the solved)
                    if(i == numcities - 1) {//this is the case where it is already on the end of the array and must draw the line to its starting point
                        SDL_RenderDrawLine(renderer, cities[best.array[i]].x, cities[best.array[i]].y, cities[best.array[0]].x, cities[best.array[0]].y);     
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
                        SDL_RenderDrawLine(renderer, cities[solved.array[i]].x-5, cities[solved.array[i]].y-5, cities[solved.array[0]].x-5, cities[solved.array[0]].y-5);
                        break;           
                    } 
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
                    SDL_RenderDrawLine(renderer, cities[solved.array[i]].x-5, cities[solved.array[i]].y-5, cities[solved.array[i+1]].x-5, cities[solved.array[i+1]].y-5); // the minus 5 pixels are just to be able to differentiate the two tracks, it still is bad though
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawLine(renderer, cities[best.array[i]].x, cities[best.array[i]].y, cities[best.array[i+1]].x, cities[best.array[i+1]].y);
            }
            
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            for (int i = 0; i < numcities; i++) {
                circle(renderer, cities[i].x, cities[i].y, 6);//draws a circle in each city coordinates
            }
            char string[300];
            sprintf(string, "Generation: %d Distance: %.3lf(%.3lf) Mutation: %d", generation, best.fitness, solved.fitness, mutation);
            SDL_Surface* textSurface = TTF_RenderText_Blended(font, string, textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);
            SDL_Rect textRect = {0, 0, 600, 25};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_RenderPresent(renderer);
            SDL_DestroyTexture(textTexture);
    }
//to compile gcc travelling.c -o travelling -lm -lSDL2 -lSDL2_ttf
    clock_t finaltime = clock();
    for(int i = 0; i < 1000000000; i++) {;} // only done to be able to visualize the algorithm's accomplishment for a few seconds before ending the execution
    double evolutivetime = (double)(finaltime-initialtime)/CLOCKS_PER_SEC;
    if((evolutivetime - bruteforcetime) > 0) {//compares both times takne by the two different approaches
        printf("The evolutive algorithm took %lf seconds more than the brute force\n", evolutivetime - bruteforcetime);
    } else {
        printf("The evolutive algorithm took %lf seconds less than the brute force one\n", bruteforcetime-evolutivetime);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    fclose(file);
    fclose(file2);
    return 0;
}
