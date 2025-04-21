#include <stdio.h>
#include <stdlib.h>

typedef struct piece{
    int indexes[4][2];
} Piece;

Piece createPiece(int pieceType){
    // 0 - 6
    Piece piece;
    switch (pieceType) {
        case 0:
            piece.indexes[0][0]=0;piece.indexes[0][1]=4;
            piece.indexes[1][0]=1;piece.indexes[1][1]=4;
            piece.indexes[2][0]=2;piece.indexes[2][1]=4;
            piece.indexes[3][0]=3;piece.indexes[3][1]=4;
            break;
        default:
            piece.indexes[0][0]=0;piece.indexes[0][1]=4;
            piece.indexes[1][0]=1;piece.indexes[1][1]=4;
            piece.indexes[2][0]=2;piece.indexes[2][1]=4;
            piece.indexes[3][0]=3;piece.indexes[3][1]=4;
            break;
    }
    return piece;
}

void updateGrid(Piece *piece, int grid[20][10]){
    for (int i = 0; i<4; i++) {
        grid[piece->indexes[i][0]][piece->indexes[i][1]] = 1;
    } 
}

void clearPiece(Piece *piece, int grid[20][10]){
    for (int i = 0; i<4; i++) {
        grid[piece->indexes[i][0]][piece->indexes[i][1]] = 0;
    }
}


int piecePhysics(Piece *piece, int grid[20][10]){
    int found = 0;
    for (int i = 0; i<4; i++) {
        // checks bellow for collision
        if(grid[piece->indexes[i][0]+1][piece->indexes[i][1]] != 0){
            found = 1;
        }
        
        if(found) break;
    }
    if(!found){

        // Update Piece down
        for (int i = 0; i<4; i++) {
            piece->indexes[i][0] += 1;
        }
    }
    return found;
}

void printGrid(int grid[20][10]){
    printf("\n");
    for (int i = 0;i<20;i++) {
        // Prints 2x2 block if there is a block there
        for (int j = 0; j < 10; j++) {
            if (grid[i][j]==0) printf(". "); else printf("##");
        }
        printf("\n");
        for (int j = 0; j < 10; j++) {
            if (grid[i][j]==0) printf(" ."); else printf("##");
        }
        printf("\n");
    }
}

void update(Piece *piece, int grid[20][10]){

    int found = 0; // collision check

    clearPiece(piece, grid);
    found = piecePhysics(piece, grid); // return whether collision is found
    updateGrid(piece,grid);
    printGrid(grid);

    // Creates a new piece if collision
    if(found){
        *piece = createPiece(0);
        updateGrid(piece,grid);
        printGrid(grid);
    }
}

int main()
{
    int grid[20][10] = {0};
    Piece piece = createPiece(0);

    while (1) {
    
        printf(">");
        scanf("%s");
        update(&piece,grid);
    }


    return 0;
}
