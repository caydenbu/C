//
// Created by Cayden Buchanan on 9/25/25.
//

#include <stdlib.h>
#include <stdio.h>
#include <arm/limits.h>

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
    node->h = abs(endNode->x - node->x) + abs(endNode->y - node->y);
    node->f = node->g + node->h;
}

void CalculateNeigbors(Node *node, Node grid[GRAPH_HEIGHT][GRAPH_WIDTH]) {
    int count = 0;

    Node **tmp = realloc(node->neighbors, 4 * sizeof *tmp);
    if (!tmp) return;          // optional: handle OOM
    node->neighbors = tmp;

    if (node->x > 0)                node->neighbors[count++] = &grid[node->y][node->x - 1];
    if (node->x < GRAPH_WIDTH - 1)  node->neighbors[count++] = &grid[node->y][node->x + 1];
    if (node->y > 0)                node->neighbors[count++] = &grid[node->y - 1][node->x];
    if (node->y < GRAPH_HEIGHT - 1) node->neighbors[count++] = &grid[node->y + 1][node->x];

    Node **shrunk = realloc(node->neighbors, count * sizeof *shrunk);
    if (shrunk) node->neighbors = shrunk;
    node->neighbor_count = count;
}

void PrintGrid(Node grid[GRAPH_HEIGHT][GRAPH_WIDTH], Node start, Node end, Node **openSet, int openLen, Node **closedSet, int closedLen) {

    // Turn openSet and closed set into a grid of ints
    int intGrid[GRAPH_HEIGHT][GRAPH_WIDTH];

    // Fill with zeros
    for (int i = 0; i < GRAPH_HEIGHT; ++i) {
        for(int j = 0; j < GRAPH_WIDTH; j++) {
            intGrid[i][j] = 0;
        }
    }
    // Fill out openSet
    for (int i = 0; i < openLen; ++i) {
        intGrid[openSet[i]->y][openSet[i]->x] = 1;
    }
    // Fill out closedSet
    for (int i = 0; i < closedLen; ++i) {
        intGrid[closedSet[i]->y][closedSet[i]->x] = 2;
    }

    // Print Grid
    for (int i = 0; i < GRAPH_HEIGHT; i++) {
        for (int j = 0; j < GRAPH_WIDTH; j++) {
            if (intGrid[i][j] == 1) {
                printf("#"); // open
            }else if (intGrid[i][j] == 2) {
                printf("*"); // closed
            }else {
                printf(".");
            }
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
            n->x = j;
            n->y = i;
            n->f = 0;
            n->g = INT8_MAX;
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
    Node **closedSet = NULL;


    while (openLen > 0) {

        int lowestWieghtIndex = 0;

        for (int i = 0; i < openLen; i++) {
            CalculateF(openSet[i], end);
            CalculateF(openSet[lowestWieghtIndex], end);
            if (openSet[i]->f < openSet[lowestWieghtIndex]->f) lowestWieghtIndex = i;
        }

        Node *current = &*openSet[lowestWieghtIndex];
        // Check if reached the end node
        if (current == end) {
            break; // path is found
        }

        CalculateNeigbors(current, grid);

        for (int i = 0; i < current->neighbor_count; ++i) {
            // if neighbor is in closed continue
            Node neighborNode = *current->neighbors[i];
            if (nodeInSet(&neighborNode, closedSet, closedLen)) {
                continue;
            }

            // Calculate all needed values for each neighbor node
            neighborNode.previous = &current;

            int tempg = current->g + 1;
            if (tempg < neighborNode.g) {
                neighborNode.g = tempg;
            }

            // Add all the neighbors to the open set
            openSet = realloc(openSet, (openLen + 1) * sizeof(Node *));
            openSet[openLen++] = &neighborNode;
        }

        // remove the current node from open to closed set
        openSet[lowestWieghtIndex] = openSet[openLen - 1];
        openLen--;
        openSet = realloc(openSet, openLen * sizeof *openSet);

        Node **cTmp = realloc(closedSet, (closedLen + 1) * sizeof *closedSet);
        if (!cTmp) break;
        closedSet = cTmp;
        closedSet[closedLen++] = current;

        PrintGrid(grid, *start, *end, openSet, openLen, closedSet, closedLen);

        printf("\n");
    }

    return 0;
}
