#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct city {
    int x;
    int y;
}; typedef struct city city;

int main(int argc, char* argv[]) {

    if(argc != 2) {
        printf("The correct use of the program is './cidades 10' i.e\n");
        return 1;
    }

    time_t t;
    srand((unsigned) time(&t));
    int numcities = atoi(argv[1]);

    FILE* file = fopen("cities.bin", "wb");

    if(file == NULL) {
        printf("File opening error\n");
        return 1;
    }

    city cities[numcities];

    for(int i = 0; i < numcities; i++) {
        cities[i].x = rand() % 601;
        cities[i].y = rand() % 601;
    }

    fwrite(cities, sizeof(city), numcities, file);

    fclose(file);
    return 0;
}
