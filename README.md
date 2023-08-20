# Travelling Salesman - Concept

Given a list of cities and the distances between each pair of cities, what is the shortest possible route that visits each city exactly once and returns to the origin city? 

This is the travelling salesman problem, the most intuitive approach to finding the soluton could be to calculate all possible routes and select the sortest. This is not a quick method though, as the number of possibles paths for n cities is $(n-1)!\over2$. Since the initial city does not matter, nor does the direction, it differs from n!. THis, however, doesnt help much.

![ezgif com-optimize](https://github.com/LVinaud/TravellingSalesman-/assets/128495824/002f5615-1540-4816-ac5e-3a7b13f34102)

An example of the evolutionary approach of mine trying to solve one situation with 15 cities.

## Why evolutionary?

The answer is simply that I am now taking the evolutionary systems discpline at college and wanted to test those conecpts in coding(C language). 

## Approach

First, I need the cities to be generate. So theres a city struct which contains a int x and an int y and I need an array of cities. Initially, those cities were being generated with random x's and y's at the beggining of the code, this was bad for comparing purposes. In order to solve this, i created a separated program to create and store in a file an array of randomly generated cities(cidades.c).
