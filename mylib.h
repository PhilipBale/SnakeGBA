#ifndef MYLIB_H
#define MYLIB_H

// Philip Bale

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned int u8;

#define OFFSET(x,y) ((x)+(y)*240) 
#define OFFSET2(x,y, height)  ((x) + (y)*(height))
#define REG_DISPCTL *(u16 *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define SCANLINECOUNTER *(volatile unsigned short*) 0x4000006

// Buttons
#define BUTTON_A (1<<0)
#define BUTTON_B (1<<1)
#define BUTTON_SELECT 1<<2)
#define BUTTON_START (1<<3)
#define BUTTON_RIGHT (1<<4)
#define BUTTON_LEFT (1<<5)
#define BUTTON_UP (1<<6)
#define BUTTON_DOWN (1<<7)
#define BUTTON_R (1<<8)
#define BUTTON_L (1<<9)

#define BUTTONS *(volatile unsigned int *)0x4000130
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

// Pixel Stuff
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define NUM_PIXELS SCREEN_WIDTH * SCREEN_HEIGHT
#define RGB(r, g, b) ((r) | (g) << 5 | (b) << 10)

#define BLACK RGB(0,0,0)
#define WHITE RGB(31, 31, 31)
#define RED RGB(31,0,0)
#define GREEN RGB(0, 31, 0)
#define BLUE RGB(0,0,31)
#define YELLOW RGB(31,31,0)
#define CYAN RGB(0,31,31)
#define MAGENTA RGB(31,0,31)
#define GRAY RGB(25, 25, 25)

extern u16 *videoBuffer;

void setPixel(int x, int y, u16 color);
void drawRect(int x, int y, int w, int h, u16 color);
void drawHollowRect(int x, int y, int w, int h, u16 color);
void plotLine(int x0, int y0, int x1, int y1, u16 color);
void waitForVBlank();
int abs(int i);
void delay(int someInt);

#endif