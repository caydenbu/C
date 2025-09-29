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

int nodeInSet(Node *node, Node **set, int length) {
    for (int i = 0; i < length; ++i) {
        if (node == set[i]) {
            return 1;
        }
    }
    return 0;
}

int main() {

    // Create Grid
    Node grid[GRAPH_HEIGHT][GRAPH_WIDTH];
    for (int i = 0; i < GRAPH_HEIGHT; i++) {
        for (int j = 0; j < GRAPH_WIDTH; j++) {
            Node *n = &grid[i][j];
            n->x = i;
            n->y = j;
            n->f = 0;
            n->g = 0;
            n->h = 0;
            n->previous = NULL;
            n->neighbors = NULL;
            n->neighbor_count = 0;
        }
    }


    Node *start = &grid[0][0];
    start->h = 0;
    Node *end = &grid[GRAPH_HEIGHT - 1][GRAPH_WIDTH - 1];

    int openLen = 1;
    Node **openSet = malloc(sizeof(Node *));
    openSet[0] = start;
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
               // if neighbor is in closed continue
                if (nodeInSet(current.neighbors[i], closedSet, closedLen)) {
                    continue;
                }

                // Calculate all needed values for each neighbor node
                Node neighborNode = *current.neighbors[i];
                neighborNode.previous = &current;

                int temph = current.h + 1;
                neighborNode.h = temph;

                // Add all the neighbors to the open set
                openSet = realloc(openSet, (openLen + 1) * sizeof(Node *));
                openSet[openLen++] = &neighborNode;
            }

            // remove the current node from open to closed set
            openLen--;
            if (openLen == 0) {
                free(openSet);
                openSet = NULL;
            }else {
                // replaces the currents position in openset with the last element
                openSet[lowestWieghtIndex] = openSet[openLen - 1];
                // removes the last element from the list
                openSet = realloc(openSet, openLen * sizeof(Node *));
            }

            // add the current node to closed set
            closedLen++;
            closedSet = realloc(closedSet, closedLen * sizeof(Node *));
            closedSet[closedLen - 1] = &current;
        }
    }

    return 0;
}
