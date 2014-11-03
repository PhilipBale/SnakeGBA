// Philip Bale

#include "mylib.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
   u8 isSet;
   int x;
   int y;
} Pos;

typedef struct  {
	int xNext;
	int yNext;
	Pos positions[SCREEN_HEIGHT * SCREEN_WIDTH];
	int posLength;
	int moveCache; // For staggered movement w/ increments
} Snake;

typedef struct {
	int x;
	int y;
	int width;
} Square;



void clearScreen();
void initSnake(Snake* ourSnake);
void incrementSnake(Snake* ourSnake, int numSpaces);
void drawSnake(Snake* ourSnake, u16 color);
void moveSnake(Snake* ourSnake);
void checkKeyDown(Snake* ourSnake);
int checkCollision(Snake* ourSnake);
int checkForSquareCollision(Snake* ourSnake, Square* square);
void assignSquare(Snake* ourSnake, Square* goal, int width);

#define BORDER_TOP 8
#define DEBUG 0

static int score = 0;

int main(void)
{
	REG_DISPCTL = MODE3 | BG2_ENABLE; 

	Snake ourSnake;
	Square ourSquare;
	int increment = 4;
	int width = 5;
	
	int scoreCount = 0;
	int scoreReset = 10;

	initSnake(&ourSnake);
	assignSquare(&ourSnake, &ourSquare, width);
	drawString(124, 0, "SNAKE | Philip Bale", RED);
	drawString(0, 0, "Score:", RED);

	while(1) {
		delay(15);
		if (scoreCount == scoreReset) {
			scoreCount = 0;
			score++;
		} else {
			scoreCount++;
		}

		checkKeyDown(&ourSnake);
		moveSnake(&ourSnake);
		drawSnake(&ourSnake, BLUE);
		if (checkCollision(&ourSnake)) {
			while(1) {
				drawString(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2, "GAME OVER", RED);
				drawString(SCREEN_WIDTH / 2 - 65, SCREEN_HEIGHT / 2 + 10, "Press A to play again", CYAN);
				if (KEY_DOWN_NOW(BUTTON_A)) {
					score = 0;
					scoreCount = 0;
					initSnake(&ourSnake);
					drawRect(0, BORDER_TOP, SCREEN_WIDTH, SCREEN_HEIGHT - BORDER_TOP, BLACK);
					break;
				}
			}
		}

		if (checkForSquareCollision(&ourSnake, &ourSquare)) {
			score *= 1.25;
			incrementSnake(&ourSnake, increment);
			assignSquare(&ourSnake, &ourSquare, width);
		}

		drawRect(75, 0, 45, BORDER_TOP, BLACK);
		Pos head = ourSnake.positions[ourSnake.moveCache];
		char c[10];
		sprintf(c, "%d,%d", head.x,head.y);
		drawString(75, 0, c, BLUE); 

		drawHollowRect(0, BORDER_TOP, SCREEN_WIDTH, SCREEN_HEIGHT - BORDER_TOP, MAGENTA);
		drawRect(ourSquare.x, ourSquare.y, ourSquare.width, ourSquare.width, GREEN);

		drawRect(35, 0, 40, BORDER_TOP, BLACK);
		sprintf(c, "%d", score);
		drawString(35, 0, c, RED); 
	 }
}

void initSnake(Snake* ourSnake) {
	ourSnake->xNext = 1;
	ourSnake->yNext = 0;
	Pos pos1 = {1, 100, 100};
	ourSnake->positions[0] = pos1;
	ourSnake->posLength = 1;
	ourSnake->moveCache = 0;
}

void incrementSnake(Snake* ourSnake, int numSpaces) {
	Pos tail = ourSnake->positions[ourSnake->posLength - 1];
	ourSnake->moveCache += numSpaces;

	for (int i = 0; i < numSpaces; i++) {
		ourSnake->positions[ourSnake->posLength + i] = tail;
	}

	ourSnake->posLength += numSpaces;
}

void drawSnake(Snake* ourSnake, u16 color) {

	for (int i = 0; i < ourSnake->posLength; i++) {
		Pos pixPos = ourSnake->positions[i];
		// Check if position set
		if (pixPos.isSet != 0) {
			setPixel(pixPos.x, pixPos.y, color);
		} else {
			break;
		}
	}
}

void moveSnake(Snake* ourSnake) {
	int moveCache = ourSnake->moveCache;
	Pos head = ourSnake->positions[0];
	Pos nextPos = {1, head.x + ourSnake->xNext, head.y + ourSnake->yNext};
	Pos lastPos = ourSnake->positions[ourSnake->posLength - ourSnake->moveCache - 1];
	 
	setPixel(lastPos.x, lastPos.y, BLACK);

	for (int i = (ourSnake->posLength - moveCache - 1); i > 0; i--) {
		ourSnake->positions[i] = ourSnake->positions[i - 1];;
	}
	
	if (moveCache > 0) {
		ourSnake->moveCache--;
	}

	ourSnake->positions[0] = nextPos;
}

void checkKeyDown(Snake* ourSnake) {
	if(KEY_DOWN_NOW(BUTTON_UP))
	{
		ourSnake->xNext = 0;
		ourSnake->yNext = -1;
	} else if(KEY_DOWN_NOW(BUTTON_DOWN))
	{
		ourSnake->xNext = 0;
		ourSnake->yNext = 1;
	} else if(KEY_DOWN_NOW(BUTTON_LEFT))
	{
		ourSnake->xNext = -1;
		ourSnake->yNext = 0;
	} else if(KEY_DOWN_NOW(BUTTON_RIGHT))
	{
		ourSnake->xNext = 1;
		ourSnake->yNext = 0; 
	}
}

int checkCollision(Snake* ourSnake) {
	Pos head = ourSnake->positions[0];

	if (head.x >= (SCREEN_WIDTH - 1) || head.x == 0) {
		return 1;
	} else if (head.y >= (SCREEN_HEIGHT - 1) || head.y <= BORDER_TOP){
		return 1;
	}

	for (int i = 1; i < (ourSnake->posLength - ourSnake->moveCache); i++) { 
		Pos tempPos = ourSnake->positions[i];
		if (tempPos.isSet != 0) {
			if (tempPos.x == head.x && tempPos.y == head.y) { 
				return 1;
			}
		} else {
			break;
		}
	}

	return 0;
}

int checkForSquareCollision(Snake* ourSnake, Square* square) {
	Pos head = ourSnake->positions[0];
	if ((head.x >= square->x && head.x <= (square->x + square->width)) && (head.y >= square->y && head.y <= (square->y + square->width))) {
		return 1;
	}

	return 0;
}

void assignSquare(Snake* ourSnake, Square* square, int width) {
	Square newSquare = { rand() % SCREEN_WIDTH, BORDER_TOP + rand() % (SCREEN_HEIGHT - BORDER_TOP), width};
	drawRect(square->x, square->y, square->width, square->width, BLACK);

	while (checkForSquareCollision(ourSnake, &newSquare)) {
		newSquare.x = BORDER_TOP + rand() % (SCREEN_WIDTH - BORDER_TOP);
		newSquare.y = rand() % SCREEN_HEIGHT;
	}

	*square = newSquare;
}