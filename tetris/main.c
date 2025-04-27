#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct piece{
    int indexes[4][2];
    int pieceType;
} Piece;

Piece createPiece(int PieceType){
    // 0 - 6
    Piece piece;
    piece.pieceType = PieceType;
    switch (PieceType) {
        case 0: // I block
            piece.indexes[0][0]=0;piece.indexes[0][1]=4;
            piece.indexes[1][0]=1;piece.indexes[1][1]=4;
            piece.indexes[2][0]=2;piece.indexes[2][1]=4;
            piece.indexes[3][0]=3;piece.indexes[3][1]=4;
            break;
        case 1: // O block
            piece.indexes[0][0]=0;piece.indexes[0][1]=3;
            piece.indexes[1][0]=0;piece.indexes[1][1]=4;
            piece.indexes[2][0]=1;piece.indexes[2][1]=3;
            piece.indexes[3][0]=1;piece.indexes[3][1]=4;
            break;
        case 2: // Z block
            piece.indexes[0][0]=1;piece.indexes[0][1]=3; // pivot piece
            piece.indexes[1][0]=0;piece.indexes[1][1]=3;
            piece.indexes[2][0]=0;piece.indexes[2][1]=2;
            piece.indexes[3][0]=1;piece.indexes[3][1]=4;
            break;
        case 3: // S block
            piece.indexes[0][0]=1;piece.indexes[0][1]=4; // pivot piece
            piece.indexes[1][0]=0;piece.indexes[1][1]=5;
            piece.indexes[2][0]=0;piece.indexes[2][1]=4;
            piece.indexes[3][0]=1;piece.indexes[3][1]=3;
            break;
        case 4: // L block
            piece.indexes[0][0]=1;piece.indexes[0][1]=4; // pivot piece
            piece.indexes[1][0]=0;piece.indexes[1][1]=4;
            piece.indexes[2][0]=2;piece.indexes[2][1]=4;
            piece.indexes[3][0]=2;piece.indexes[3][1]=5;
            break;
        case 5: // J block
            piece.indexes[0][0]=1;piece.indexes[0][1]=4; // pivot piece
            piece.indexes[1][0]=0;piece.indexes[1][1]=4;
            piece.indexes[2][0]=2;piece.indexes[2][1]=4;
            piece.indexes[3][0]=2;piece.indexes[3][1]=3;
            break;
        case 6: // T block
            piece.indexes[0][0]=0;piece.indexes[0][1]=4; // pivot piece
            piece.indexes[1][0]=0;piece.indexes[1][1]=5;
            piece.indexes[2][0]=0;piece.indexes[2][1]=3;
            piece.indexes[3][0]=1;piece.indexes[3][1]=4;
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

void leftRight(Piece *piece, int grid[20][10], int dir){
    int found = 0;

    // if input down or is rotation then dont move left or right
    if(abs(dir) == 2 || dir == 0) return;

    for (int i = 0; i<4; i++) {
        // checks towards direction for collision
        if(grid[piece->indexes[i][0]][piece->indexes[i][1]+dir] != 0){
            found = 1;
        }
        
        if(found) break;
    }
    if(!found){
        // Update piece towards direction
        for (int i = 0; i<4; i++) {
            piece->indexes[i][1] += dir;
        }
    }
}

void printGrid(int grid[20][10]){

    // Prints buffer between each new screen
    for (int i = 0; i<1; i++) {
        printf("\n");
    }

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

void rotate(Piece *piece, int grid[20][10], int dir){
    // I block needs special rotation
    if(piece->pieceType == 0){

    }
    // O block can be ignored it doesnt rotate
    if(piece->pieceType == 1) return;

    int pRow = piece->indexes[0][0];
    int pCol = piece->indexes[0][1];

    // rotation check
    int found = 0;
    for (int i = 0; i < 4; i++) {
        int row = piece->indexes[i][0];
        int col = piece->indexes[i][1];

        int reletiveRow = row -pRow;
        int reletiveCol = col -pCol;

        int newReletiveRow;
        int newReletiveCol;

        if(dir == 2){
            newReletiveRow = -reletiveCol;
            newReletiveCol = reletiveRow;
        }else{
            newReletiveRow = reletiveCol;
            newReletiveCol = -reletiveRow;
        }

        row = pRow + newReletiveRow;
        col = pCol + newReletiveCol;

        if(grid[row][col]==1){
            found = 1;
        }
    }
    
    // Stops rotation if found block in the way
    if(found) return;

    for (int i = 0; i < 4; i++) {
        int *row = &piece->indexes[i][0];
        int *col = &piece->indexes[i][1];

        int reletiveRow = *row -pRow;
        int reletiveCol = *col -pCol;

        int newReletiveRow;
        int newReletiveCol;

        if(dir == 2){
            newReletiveRow = -reletiveCol;
            newReletiveCol = reletiveRow;
        }else{
            newReletiveRow = reletiveCol;
            newReletiveCol = -reletiveRow;
        }

        *row = pRow + newReletiveRow;
        *col = pCol + newReletiveCol;
    }
}

void update(Piece *piece, int grid[20][10], int dir){


    int foundGround = 0; // collision check
    clearPiece(piece, grid);

    // movements and rotation
    leftRight(piece, grid, dir);
    if(dir==0) foundGround = piecePhysics(piece, grid);
    if(abs(dir)== 2) rotate(piece, grid, dir);

    
    updateGrid(piece,grid);
    printGrid(grid);

    // Creates a new piece if collision
    if(foundGround){
        *piece = createPiece(rand()%7);
        updateGrid(piece,grid);
        printGrid(grid);
    }
}

int main()
{
    // sets random seed
    srand(time(NULL));

    int grid[20][10] = {0};
    Piece piece = createPiece(rand()%7);

    while (1) {
    
        char input[10];
        int value = 0;

        printf(">");
        fgets(input, sizeof(input), stdin);

        // -1, 0, 1: movement, -2, 2: rotation
        if(input[0] == 'a'){
            value = -1;
        }
        else if(input[0] == 'd'){
            value = 1;
        }
        else if(input[0] == 's'){
            value = 0;
        }
        else if(input[0] == 'q'){
            value = -2;
        }
        else if(input[0] == 'e'){
            value = 2;
        }

        update(&piece,grid, value);
    }


    return 0;
}
