//
// Created by Cayden Buchanan on 9/25/25.
//

#include <stdlib.h>
#include <stdio.h>

void PrintGrid(int grid[10][10], int startIndex[2], int endIndex[2]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (startIndex[0]==i && startIndex[1]==j) {
                printf("# ");
            }else if (endIndex[0]==i && endIndex[1]==j) {
                printf("X ");
            }else {
                printf("%d", grid[i][j]);
                printf(" ");
            }
        }
        printf("\n");
    }
}

int calculateDistance(int currentNode[2], int endNode[2]) {
    return abs(endNode[0] - currentNode[0]) + abs(endNode[1] - currentNode[1]);
}

int main() {

    // Create Grid
    int grid[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j] = 0;
        }
    }

    // set the Start indexes
    int startIndex[2] = {2,1};
    int endIndex[2] = {6,7};

    // Start search
    int currentNode[2] = {startIndex[0], startIndex[1]};
    currentNode[0]++;
    currentNode[1]++;

    int dist = calculateDistance(currentNode, endIndex);
    grid[currentNode[0]][currentNode[1]] = dist;


    // Update Grid
    PrintGrid(grid, startIndex, endIndex);

    return 0;
}
