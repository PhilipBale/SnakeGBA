// Philip Bale
#include "mylib.h"

u16 *videoBuffer = (u16 *)0x6000000;

// A function to set pixel (x, y) to the color passed in.
void setPixel(int x, int y, u16 color)
{
	*(videoBuffer + OFFSET(x, y)) = color;
}

// A function to draw a FILLED rectangel starting at (x, y)
void drawRect(int x, int y, int w, int h, u16 color)
{
	//Iterate through each pixel
	for (int i = x; i < (x + w); i++)
	{
		for (int j = y; j < (y + h); j++) 
		{
			setPixel(i, j, color);
		}
	}
}

// A function to draw a HOLLOW rectangel starting at (x, y)
// NOTE: It has to run in O(w+h) time
void drawHollowRect(int x, int y, int w, int h, u16 color)
{
	for (int i = x; i < (w + x); i++) 
	{
		setPixel(i, y, color);
		setPixel(i, y + h - 1, color);
	}

	for (int j = (y + 1); j < (h + y - 1); j++)
	{
		setPixel(x, j, color);
		setPixel(x + w - 1, j, color);
	}
}

// A function to draw a line using Bresenham's algorithm
void plotLine(int x0, int y0, int x1, int y1, u16 color)
{
	// Swap so we are going left to right
	if (x0 > x1) {
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	} 


	int dx = abs(x1 - x0); // Change x
	int sx = x0 < x1 ? 1 : -1; // Sign x

	int dy = -abs(y1 - y0); // Change y
	int sy = y0 < y1 ? 1 : -1; // Sign y

	int error1 = dx+dy; // Calc total change
	int error2 = error1 << 1; // Mult by 2

	while(1)
	{
		setPixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) {
			break;
		}

		error2 = error1 << 1;
		if (error2 >= dy)
		{
			error1 += dy;
			x0 += sx;
		}

		if (error2 <= dx) {
			error1 += dx;
			y0 += sy;
		}
	}
}

int abs(int x) {
	return x >= 0 ? x : -x;
}

void waitForVBlank()
{
	while(SCANLINECOUNTER>160);
	while(SCANLINECOUNTER<160);
}

void delay(int someInt) {
	volatile int n = 0;
	while (n < someInt) {
		waitForVBlank();
		n++;
	}
}
