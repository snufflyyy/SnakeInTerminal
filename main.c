/**
 * Terminal Snake
 * written by snuffly in c
 * last modified on 3/22/2024
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h> // windows only :(
#include <time.h>

// in c arrays must know their size at compile time
// 30 "tiles" in the x and 15 "tiles" in the y
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

// is the game over?
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

// the current snakes direction
Direction snakeDirection;

// the snakes tail
// YSIZE - 1 * XSIZE - 1 is the largest the snakes tail can possibly get
Position snakeTail[(YSIZE - 1) * (XSIZE - 1)];

// always the initialize function to use the spawnApple function
void spawnApple();

// sets default values and other related needs
void initialize() {
    gameOver = false;

    // puts the snakes head position in the middle of the game board
    snakeHeadPos.x = XSIZE / 2;
    snakeHeadPos.y = YSIZE / 2;
    snakeDirection = NONE;

    spawnApple();
}

void restart() {
    gameOver = false;

    // clears the old snake tail
    for (int i = 0 ; i < score; i++) {
        snakeTail[i].x = 0;
        snakeTail[i].y = 0;
    }

    // sets the score back to 0
    score = 0;

    // puts the snakes head position in the middle of the game board
    snakeHeadPos.x = XSIZE / 2;
    snakeHeadPos.y = YSIZE / 2;
    snakeDirection = NONE;

    spawnApple();
}

// gets keyboard input from the user
void input() {
    // checks if the keyboard was even hit
    if (kbhit() != 0) {
        // the arrow keys in getch() first returns a "224" then the arrow key code
        if (getch() == 224) {
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

            // snake tail
            // ignoring index 0 because that is where the snake's head will always be
            for (int i = 1; i < score; i++) {
                // checking if the snake's tail is equal to 0 in any direction is to prevent a visual bug
                // where the top left corner (0,0) is replaced with a snake tail instead of the border "tile"
                if (x == snakeTail[i].x && y == snakeTail[i].y && snakeTail[i].x != 0 && snakeTail[i].y != 0) {
                    gameBoard[y][x] = 3;
                }
            }
        }
    }
}

// spawns the apple
void spawnApple() {
    // resets the apple position to (0,0)
    applePos.x = 0;
    applePos.y = 0;

    if (score == 0) {
        // checks if the position is (0,0) or border and if it is then regenerate the apple pos
        while ((applePos.x == 0 || applePos.y == 0) || (applePos.x == XSIZE - 1 || applePos.y == YSIZE - 1)) {
            srand(time(NULL));
            applePos.x = rand() % XSIZE;
            applePos.y = rand() % YSIZE;
        }
    } else {
        for (int i = 1; i < score; i++) {
            // checks if the position is (0,0), the border, or if it is inside the tail and if it is then regenerate the apple pos
            while ((applePos.x == 0 || applePos.y == 0) || (applePos.x == XSIZE - 1 || applePos.y == YSIZE - 1) || (applePos.x == snakeTail[i].x && applePos.y == snakeTail[i].y)) {
                srand(time(NULL));
                applePos.x = rand() % XSIZE;
                applePos.y = rand() % YSIZE;
            }
        }
    }
}

// updates the snakes tail
void updateTail() {
    // temp variables
    Position lastPos = snakeTail[0];
    Position lastPos2;

    // sets the first tail object to be where the snakes head is
    snakeTail[0].x = snakeHeadPos.x;
    snakeTail[0].y = snakeHeadPos.y;

    // starts at the one and sets updates the positions
    for (int i = 1; i < score; i++) {
        lastPos2 = snakeTail[i];
        snakeTail[i].x = lastPos.x;
        snakeTail[i].y = lastPos.y;
        lastPos = lastPos2;
    }
}

void checkSnake() {
    // checks to see if the snakes head touched a border "tile"
    if (gameBoard[snakeHeadPos.y][snakeHeadPos.x] == 1) {
        gameOver = 1;
    }

    // checks to see if the snakes head touched the apple
    if (gameBoard[snakeHeadPos.y][snakeHeadPos.x] == 4) {
        score += 2;

        spawnApple();
    }

    // checks if the snakes head touches the snakes tail
    for (int i = 1; i < score; i++) {
        if (snakeTail[i].x == snakeHeadPos.x && snakeTail[i].y == snakeHeadPos.y) {
            gameOver = true;
        }
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

    checkSnake();

    updateTail();

    updateBoard();
}

// renders the game board
void render() {
    // clearing the terminal
    system("cls");

    if (!gameOver) {
        printf("Score: %d\n", score);
    } else {
        printf("Game Over :( - Press R to Play Again\n");
    }

    // renders the board
    for (int y = 0; y < YSIZE; y++) {
        for (int x = 0; x < XSIZE; x++) {
            switch (gameBoard[y][x]) {
                case 0: printf(" "); break; // empty
                case 1: printf("#"); break; // border
                case 2: printf("O"); break; // snake head
                case 3: printf("o"); break; // snake body
                case 4: printf("*"); break; // apple
            }
        }
        // skips to the next line
        printf("\n");
    }
}

int main() {
    initialize();

    // main game loop, run until game over
    while (1) {
        while (!gameOver) {
            update();
            render();
        }
    }

    return 0;
}
