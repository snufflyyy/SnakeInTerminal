#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>

// OS can be "windows", "linux", or "macOS"
// this is because windows and linux
// have different commands to clear the screen
#define OS "windows"

// in c arrays must know their size at compile time
// 50 "tiles" in the x and 15 "tiles" in the y
#define XSIZE 30
#define YSIZE 15

// x and y values put into a single type
typedef struct {
    int x;
    int y;
} Position;

// the possible directions of the snake
typedef enum {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// 0 is empty space, 1 a border, 2 is the snake head, 3 is the snakes body, and 4 is an apple
int gameBoard[YSIZE][XSIZE];

// 0 is not game over, 1 is game over, just a boolean
bool gameOver;

// score of the game
int score;

// the position of the apple
Position applePos;

///////////////////////////////
// the snake                 //
///////////////////////////////

// the position of the snake head, default is the middle of the game board
Position snakeHeadPos;

// the body of the snake
Position *snakeBody;

// the current snakes direction
Direction snakeDirection;

void spawnApple();

// gets keyboard input from the user
void input() {
    // checks if the keyboard was even hit
    if (kbhit() != 0) {
        // the arrow keys in getch() first returns a "224" then the arrow key code
        if(getch() == 224) {
            switch (getch()) {
                case 72: if (snakeDirection != DOWN) snakeDirection = UP; break;
                case 80: if (snakeDirection != UP) snakeDirection = DOWN; break;
                case 75: if (snakeDirection != RIGHT) snakeDirection = LEFT; break;
                case 77: if (snakeDirection != LEFT) snakeDirection = RIGHT; break;
            }
        }
    }
}

// updates the game board
void updateBoard() {
    for (int y = 0; y < YSIZE; y++) {
        for (int x = 0; x < XSIZE; x++) {
            // checks to see if the current "tile" is at the edge
            // if it is then it sets it as a border "tile"
            if (x == 0 || y == 0 || x == XSIZE - 1 || y == YSIZE - 1) {
                // border
                gameBoard[y][x] = 1;
            } else if (x == snakeHeadPos.x && y == snakeHeadPos.y) {
                // snake head
                gameBoard[y][x] = 2;
            } else if (x == applePos.x && y == applePos.y) {
                gameBoard[y][x] = 4;
            } else {
                // empty space (this isn't needed because ints start at 0)
                gameBoard[y][x] = 0;
            }
        }
    }
}

// spawns the apple
void spawnApple() {
    // resets the apple position to (0,0)
    applePos.x = 0;
    applePos.y = 0;

    // checks if the position is (0,0) or (XSIZE - 1, YSIZE - 1) and if it is then regenerate the apple pos
    while ((applePos.x == 0 || applePos.y == 0) || (applePos.x == XSIZE - 1 || applePos.y == YSIZE - 1)) {
        srand(time(NULL));
        applePos.x = rand() % XSIZE;
        applePos.y = rand() % YSIZE;
    }
}

// update the game
void update() {
    input();

    // updates the snakes position based on the snake direction
    // movement speed is based on computer speed but this is a
    // simple game, so it really isn't that big of an issue
    switch (snakeDirection) {
        case NONE: /** do nothing **/ break;
        case UP: snakeHeadPos.y--; break;
        case DOWN: snakeHeadPos.y++; break;
        case LEFT: snakeHeadPos.x--; break;
        case RIGHT: snakeHeadPos.x++; break;
    }

    // checks to see if the snakes head touched a border "tile"
    if (gameBoard[snakeHeadPos.y][snakeHeadPos.x] == 1) {
        gameOver = 1;
    }

    // checks to see if the snakes head touched the apple
    if (gameBoard[snakeHeadPos.y][snakeHeadPos.x] == 4) {
        if (score <= 0) {
            score += 5;
            // creates an "array" that has score's amount of slots
            snakeBody = malloc(score * sizeof(Position));
        } else {
            score += 5;

            // changes the size
            Position *temp = realloc(snakeBody, score * sizeof(Position));
            // error checking
            if (temp) {
                snakeBody = temp;
            }
        }

        spawnApple();
    }

    updateBoard();
}

// renders the game board
void render() {
    // checks to see what OS is selected, this is because
    // linux (and probably macOS) have a different way of
    // clearing the terminal then windows does
    if (OS == "windows") {
        system("cls");
    } else if (OS == "linux" || OS == "macOS") {
        printf("\033[H\033[J");
    } else {
        gameOver = 1;
        printf("Error: Operating System is not valid!\n");
        printf("Try changing the OS definition at the top of the file to a valid operating system\n");
    }

    // renders the board
    for (int y = 0; y < YSIZE; y++) {
        for (int x = 0; x < XSIZE; x++) {
            switch (gameBoard[y][x]) {
                case 1: printf("#"); break; // border
                case 2: printf("O"); break; // snake head
                case 3: printf("o"); break; // snake body
                case 4: printf("*"); break; // apple
                default: printf("."); break; // empty
            }
        }
        // skips to the next line
        printf("\n");
    }
}

int main() {
    gameOver = false;

    // puts the snakes head position in the middle of the game board
    snakeHeadPos.x = XSIZE / 2;
    snakeHeadPos.y = YSIZE / 2;

    snakeDirection = NONE;

    spawnApple();


    // main game loop, run until game over
    while (1) {
        while (gameOver == false) {
            update();
            render();
        }
    }

    return 0;
}
