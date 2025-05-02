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

void intro(){
    printf("Welcome To Tetris\n");
    printf("A,S,D to move, and Q,E to rotate\n");
    printf("press enter to start:");
    getchar();
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
        if(grid[piece->indexes[i][0]+1][piece->indexes[i][1]] != 0 || piece->indexes[i][0]+1 > 19){
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

    // if input down or is rotation or hard drop then dont move left or right
    if(abs(dir) == 2 || dir == 0 || dir == 3) return;

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

void printGrid(int grid[20][10], int score){

    // Prints buffer between each new screen
    for (int i = 0; i<20; i++) {
        printf("\n");
    }
    printf("Score: ");
    printf("%d\n", score);

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

void clearLines(int grid[20][10], int *score){

    // checks every line for full set of 1s 
    for (int i = 0; i<20; i++) {
        int found = 1;
        // Checks if line is full
        for (int j = 0; j<10; j++) {
            if(grid[i][j]==0){
                found = 0;
            }
        }
        // If line is full
        if(found){

            // lower everything above that line
            for (int j=i; j>0; j--) {
                for (int k=0; k<10; k++) {
                    grid[j][k] = grid[j-1][k];
                }
            }
            // Clear top row
            for (int j=0; j<10; j++) {
                grid[0][j] = 0;
            }
            

            *score += 100;
            i--; // recheck after moving everything down?
        }
    }
}

int didLose(Piece *piece, int grid[20][10]){

    int found = 0;
    for (int i = 0; i<4; i++) {
        // checks bellow for collision
        if(grid[piece->indexes[i][0]][piece->indexes[i][1]] != 0){
            found = 1;
        }
        
        if(found) break;
    }
    return found;
}

int update(Piece *piece, int grid[20][10], int dir, int *score){

    int foundGround = 0; // collision check
    clearPiece(piece, grid);

    // movements and rotation
    leftRight(piece, grid, dir);
    if(dir==0) foundGround = piecePhysics(piece, grid); // if no left right then go down
    if(abs(dir)== 2) rotate(piece, grid, dir); // if its a rotation dir then rotate
    // hard drop
    if(dir==3){
        while(1){
            foundGround = piecePhysics(piece, grid); 
            if(foundGround) break;
        }
    }
    updateGrid(piece,grid);
    printGrid(grid, *score);

    if(foundGround){

        clearLines(grid, score);
        // Creates a new piece if collision
        *piece = createPiece(rand()%7);

        if(didLose(piece, grid)){
            return 1;
        }

        updateGrid(piece,grid);
        printGrid(grid, *score);
    }
    return 0;
}

int main()
{
    intro();

    // sets random seed
    srand(time(NULL));

    int score = 0;
    int grid[20][10] = {0};
    Piece piece = createPiece(rand()%7);

    while (1) {
    
        char input[10];
        int value = 0;

        printf(">");
        fgets(input, sizeof(input), stdin);

        // -1, 0, 1: movement, -2, 2: rotation, 3: hard drop
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
        }else if(input[0]=='w'){
            value = 3;
        }

        int didLose = update(&piece, grid, value, &score);
        // if you lose then break
        if(didLose){
            printf("you lost :( Score: ");
            printf("%d\n", score);
            break;
        }
    }

    return 0;
}
