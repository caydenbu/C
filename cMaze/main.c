//
// Created by Cayden Buchanan on 9/25/25.
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

// Globals
const int GRAPH_WIDTH = 50;
const int GRAPH_HEIGHT = 50;
const int TOTAL_WALLS = 500;

typedef struct Node {
    int x;
    int y;

    int f;
    int g;
    int h;

    int isWall;

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

    if (node->isWall) {
        free(node->neighbors);
        node->neighbors = NULL;
        node->neighbor_count = 0;
        return;
    }

    Node **tmp = realloc(node->neighbors, 4 * sizeof *tmp);
    if (!tmp) {
        node->neighbor_count = 0;
        return;
    }
    node->neighbors = tmp;

    // Left
    if (node->x > 0 && !grid[node->y][node->x - 1].isWall)
        node->neighbors[count++] = &grid[node->y][node->x - 1];

    // Right
    if (node->x < GRAPH_WIDTH - 1 && !grid[node->y][node->x + 1].isWall)
        node->neighbors[count++] = &grid[node->y][node->x + 1];

    // Up
    if (node->y > 0 && !grid[node->y - 1][node->x].isWall)
        node->neighbors[count++] = &grid[node->y - 1][node->x];

    // Down
    if (node->y < GRAPH_HEIGHT - 1 && !grid[node->y + 1][node->x].isWall)
        node->neighbors[count++] = &grid[node->y + 1][node->x];

    // Shrink to actual count (or free if none)
    if (count > 0) {
        Node **shrunk = realloc(node->neighbors, count * sizeof *shrunk);
        if (shrunk) node->neighbors = shrunk;
        node->neighbor_count = count;
    } else {
        free(node->neighbors);
        node->neighbors = NULL;
        node->neighbor_count = 0;
    }
}


void PrintGrid(Node grid[GRAPH_HEIGHT][GRAPH_WIDTH], Node start, Node end, Node **openSet, int openLen, Node **closedSet, int closedLen) {

    // Turn openSet and closed set into a grid of ints
    int intGrid[GRAPH_HEIGHT][GRAPH_WIDTH];

    // Fill with zeros
    for (int i = 0; i < GRAPH_HEIGHT; ++i) {
        for(int j = 0; j < GRAPH_WIDTH; j++) {
            if (grid[j][i].isWall) {
                intGrid[i][j] = 3;
            }
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
            if (grid[i][j].isWall) {
                printf("# ");  // wall
            }else if (intGrid[i][j] == 1) {
                printf("o "); // open
            }else if (intGrid[i][j] == 2) {
                printf("* "); // closed
            }else {
                printf(". ");
            }
        }
        printf("\n");
    }
}


void PrintPath(Node *endNode, Node grid[GRAPH_HEIGHT][GRAPH_WIDTH]) {
    int intGrid[GRAPH_HEIGHT][GRAPH_WIDTH];

    for (int i = 0; i < GRAPH_HEIGHT; i++) {
        for (int j = 0; j < GRAPH_WIDTH; j++) {
            intGrid[i][j] = 0;
        }
    }

    // trace backward
    Node *p = endNode;
    while (p != NULL) {
        intGrid[p->y][p->x] = 1;
        p = p->previous;
    }

    // print the path grid
    for (int i = 0; i < GRAPH_HEIGHT; i++) {
        for (int j = 0; j < GRAPH_WIDTH; j++) {
            if (grid[i][j].isWall) {
                printf("# ");  // wall
            }else if (intGrid[i][j] == 1) {
                printf("o ");   // path cell
            } else {
                printf(". ");   // empty cell
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

void CreateWalls(Node grid[GRAPH_HEIGHT][GRAPH_WIDTH], Node *start, Node *end) {
    srand(time(NULL)); // seed randomness once

    for (int i = 0; i < TOTAL_WALLS; i++) {
        int randY = rand() % GRAPH_HEIGHT;
        int randX = rand() % GRAPH_WIDTH;

        Node *cell = &grid[randY][randX];

        // skip if it's the start, end, or already a wall
        if (cell == start || cell == end || cell->isWall == 1) {
            i--; // redo this wall placement
            continue;
        }

        cell->isWall = 1;
    }
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
            n->isWall = 0;
            n->previous = NULL;
            n->neighbors = NULL;
            n->neighbor_count = 0;
        }
    }


    Node *start = &grid[0][0];
    start->g = 0;
    Node *end = &grid[GRAPH_HEIGHT - 1][GRAPH_WIDTH - 1];

    CreateWalls(grid, start, end);

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
            PrintPath(current, grid);
            break; // path is found
        }

        CalculateNeigbors(current, grid);

        for (int i = 0; i < current->neighbor_count; ++i) {
            Node *neighbor = current->neighbors[i];

            if (nodeInSet(neighbor, closedSet, closedLen)) {
                continue;
            }

            int tentativeG = current->g + 1;
            if (!nodeInSet(neighbor, openSet, openLen)) {
                // first time seen
                neighbor->previous = current;
                neighbor->g = tentativeG;
                CalculateF(neighbor, end);

                openSet = realloc(openSet, (openLen + 1) * sizeof(Node *));
                openSet[openLen++] = neighbor;
            } else if (tentativeG < neighbor->g) {
                // found a better path
                neighbor->previous = current;
                neighbor->g = tentativeG;
            }
        }

        // remove the current node from open to closed set
        openSet[lowestWieghtIndex] = openSet[openLen - 1];
        openLen--;
        openSet = realloc(openSet, openLen * sizeof *openSet);

        // Wierd ass pointer stuff
        Node **cTmp = realloc(closedSet, (closedLen + 1) * sizeof *closedSet);
        if (!cTmp) break;
        closedSet = cTmp;
        closedSet[closedLen++] = current;

        PrintGrid(grid, *start, *end, openSet, openLen, closedSet, closedLen);

        printf("\n");
    }

    return 0;
}

