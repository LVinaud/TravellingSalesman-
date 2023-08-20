# Travelling Salesman - Concept

Given a list of cities and the distances between each pair of cities, what is the shortest possible route that visits each city exactly once and returns to the origin city? 

This is the travelling salesman problem, the most intuitive approach to finding the soluton could be to calculate all possible routes and select the sortest. This is not a quick method though, as the number of possibles paths for n cities is $(n-1)!\over2$. Since the initial city does not matter, nor does the direction, it differs from n!. THis, however, doesnt help much.

![ezgif com-optimize](https://github.com/LVinaud/TravellingSalesman-/assets/128495824/002f5615-1540-4816-ac5e-3a7b13f34102)

An example of the evolutionary approach of mine trying to solve one situation with 15 cities.

## Why evolutionary?

The answer is simply that I am now taking the evolutionary systems discpline at college and wanted to test those conecpts in coding(C language). 

## Approach

First, I need the cities to be generated. There's a city struct which contains an int x and an int y and I need an array of cities. Initially, those cities were being generated with random x's and y's at the beggining of the code, this was bad for comparing purposes. In order to solve this, I created a separated program to create and store in a file an array of randomly generated cities(cidades.c). It's importante to notice that I am using argc and argv in those programs, and the argv argument of cidades.c is the number of cities which is also gonna be stored in a file for posterior reading.

Also, it's important to be able to compare to an actual solid answer. That being said, a brute force algorithm was built for comparing porpuses, as it is extremely inefficient for reasons already explained. This brute force algorithm writes a file containing the perfect individual and also the time it took to calculate. (12 cities is taking around 50 seconds) (bruteforce.c)

Now the main code reads from that file to start.

Randomly initialize the population with a given number of individuals by creating a population array of individual structures which contain an int array and a double fitness(to store the distance of its path). After this, their arrays are shuffled.

Score each individual and the one with the lowest fitness or distance is copied to the variable best.
The best won't die nor be mutated until a new one is found.

Mutation - There's a lot of ways this can happen, the most simple might be to randomly choose two cities to swap places. A mutation rate might even increase how many times this happens to each individual.

The scoring, selection and mutation processes repeat until the number of generations is met.
You might be thinking I forgot about crossover, but it was intentionally left out as it was most proeminently just generating random beings. The code without the graphics and with the most basics strategies can be seen in travelling.c
