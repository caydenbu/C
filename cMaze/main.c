//
// Created by Cayden Buchanan on 9/25/25.
//

#include <stdlib.h>
#include <stdio.h>

// Globals
const int GRAPH_WIDTH = 10;
const int GRAPH_HEIGHT = 10;

typedef struct Node {
    int x;
    int y;

    int f; // wieght of node
    int g; // distance from finish
    int h; // distance from start

    struct Node *previous;
    struct Node **neighbors;
    int neighbor_count;
} Node;


void CalculateF(Node* node, Node *endNode) {
    // calculate distance from node to end
    node->g = abs(endNode->x - node->x) + abs(endNode->y - node->y);
    if (!node->h) node->h = 0; // incase node is uninitialized for some reason
    node->f = node->g + node->h;
}

void CalculateNeigbors(Node *node, Node grid[GRAPH_HEIGHT][GRAPH_WIDTH]) {
    int count = 0;

    Node **tmp = realloc(node->neighbors, 4 * sizeof *tmp);
    node->neighbors = tmp;

    if (node->x > 0)   node->neighbors[count++] = &grid[node->x - 1][node->y];
    if (node->x < GRAPH_WIDTH-1)   node->neighbors[count++] = &grid[node->x + 1][node->y];
    if (node->y > 0)   node->neighbors[count++] = &grid[node->x][node->y - 1];
    if (node->y < GRAPH_HEIGHT-1)   node->neighbors[count++] = &grid[node->x][node->y + 1];

    // shrink to exact size (optional)
    Node **shrunk = realloc(node->neighbors, count * sizeof *shrunk);
    if (shrunk) node->neighbors = shrunk;

    node->neighbor_count = count;
}

void PrintGrid(int grid[10][10], int startIndex[2], int endIndex[2]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
        }
        printf("\n");
    }
}

int main() {

    // Create Grid
    Node grid[GRAPH_HEIGHT][GRAPH_WIDTH];
    for (int i = 0; i < GRAPH_HEIGHT; i++) {
        for (int j = 0; j < GRAPH_WIDTH; j++) {
            Node node;
            node.y = i;
            node.x = j;
            grid[i][j] = node;
        }
    }


    Node *start = &grid[0][0];
    Node *end = &grid[GRAPH_HEIGHT - 1][GRAPH_WIDTH - 1];

    int openLen = 1;
    Node **openSet = {start};
    int closedLen = 0;
    Node **closedSet;


    while (openSet) {
        int lowestWieghtIndex = 0;
        for (int i = 0; i < openLen; i++) {

            // Pick the lowest weight of all open set
            CalculateF(openSet[i], end);
            if (openSet[i]->f < openSet[lowestWieghtIndex]->f) {
                lowestWieghtIndex = i;
            }

            // Check if reached the end node
            if (openSet[lowestWieghtIndex] == end) {
                break; // path is found
            }

            Node current = *openSet[lowestWieghtIndex];
            CalculateNeigbors(&current, grid);

            for (int i = 0; i < current.neighbor_count; ++i) {
               // if neightbor is in closed continue

                // else add all neighbors to open
                // neighbors h = current += 1 and neighbors previous = current

            }
        }
    }

    return 0;
}
