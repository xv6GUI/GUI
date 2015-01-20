#include "types.h"
#include "BACKGROUND.h"
#include "FRAME.h"
#include "icon.h"
#include "defs.h"
#include "gui.h"

static unsigned short SCREEN_COLOR[SCREEN_WIDTH][SCREEN_HEIGHT];
static unsigned short mouseDeskOri[MOUSE_WIDTH][MOUSE_HEIGHT];

static unsigned short *VESA_ADDR = (unsigned short*)0xe0000000;
static uint mousePosX = SCREEN_WIDTH / 2;
static uint mousePosY = SCREEN_HEIGHT / 2; 

//event of Render
void renderScreen(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	int i, j;
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			unsigned int offset = (y + j) * SCREEN_WIDTH + (x + i);
			*(VESA_ADDR + offset) = SCREEN_COLOR[x + i][y + j];
		}
	}
}

void drawPoint(unsigned int x, unsigned int y, unsigned short color){
	if (
		(x >= SCREEN_WIDTH) || 
		(y >= SCREEN_HEIGHT)
		)
		return;
	SCREEN_COLOR[x][y] = color;
}

//event of Mouse
void cleanMouse(){
       int i, j;
	for (i = 0; i < MOUSE_WIDTH; i++){
		for (j = 0; j < MOUSE_HEIGHT; j++){
			unsigned int offset = (j + mousePosY) * SCREEN_WIDTH + (i + mousePosX);
			*(VESA_ADDR + offset) = mouseDeskOri[i][j];
                        
		}
	}
}

void drawMouse(int posX, int posY){
        cleanMouse();
	
	int i, j;
	for (i = 0; i < MOUSE_WIDTH; i++){
		for (j = 0; j < MOUSE_HEIGHT; j++){
			unsigned int offset = (j + posY) * SCREEN_WIDTH + (i + posX);
			mouseDeskOri[i][j] = *(VESA_ADDR + offset);
			*(VESA_ADDR + offset) = 0xfff;
		}
	}
	mousePosX = posX;
	mousePosY = posY;
}


void drawBackground(int id)
{
	int i, j;
	for (i = 0; i < SCREEN_WIDTH; i++){
		for (j = 0; j < SCREEN_HEIGHT; j++)
			drawPoint(i, j, BACKGROUND[id][i][j]);
	}
}

void drawIcon(int id, int posX, int posY){
	int i, j;
	for (i = 0; i < ICON_WIDTH; i++){
		for (j = 0; j < ICON_HEIGHT; j++){
                        if(icon[id][i][j] < 100)   continue;
			SCREEN_COLOR[posX + i][posY + j] = icon[id][i][j];
		}
	}
}

void drawWindow(int id, int posX, int posY){
	int i, j;
	for (i = 0; i < WINDOW_WIDTH; i++){
		for (j = 0; j < WINDOW_HEIGHT; j++){
			SCREEN_COLOR[posX + i][posY + j] = WINDOW_FRAME[id][i][j];
		}
	}
}

//init and redraw
void renderGUI(int id)
{
	drawBackground(id);
	renderScreen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	drawMouse(mousePosX, mousePosY);
}

void initGUI()
{
	drawBackground(0);
	drawIcon(0, ICON_X1, ICON_Y1);
	drawIcon(1, ICON_X1, ICON_Y2);
	drawIcon(2, ICON_X1, ICON_Y3);
	drawIcon(3, ICON_X1, ICON_Y4);
	drawIcon(4, ICON_X1, ICON_Y5);
	drawIcon(6, ICON_X2, ICON_Y1);
	//drawWindow(0, WINDOW_X, WINDOW_Y);
	renderScreen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        int i, j;
	for (i = 0; i < MOUSE_WIDTH; i++){
		for (j = 0; j < MOUSE_HEIGHT; j++){
			uint offset = (j + mousePosY) * SCREEN_WIDTH + (i + mousePosX);
			mouseDeskOri[i][j] = *(VESA_ADDR + offset);
		}
	}
	drawMouse(mousePosX,mousePosY);
}
