#include "types.h"
#include "BACKGROUND.h"
#include "FRAME.h"
#include "defs.h"
#include "gui.h"
#include "win32/word.h"
#include "win32/icon.h"
#include "win32/file.h"
#include "win32/cursor.h"
#include "win32/cursor_help.h"

static unsigned short SCREEN_COLOR[SCREEN_WIDTH][SCREEN_HEIGHT];
static unsigned short mouseDeskOri[CURSOR_WIDTH][CURSOR_HEIGHT];

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
	if ( (x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGHT))    return;
	SCREEN_COLOR[x][y] = color;
}

//event of Mouse
void cleanMouse(){
       int i, j;
	for (i = 0; i < CURSOR_WIDTH; i++){
		for (j = 0; j < CURSOR_HEIGHT; j++){
			unsigned int offset = (j + mousePosY) * SCREEN_WIDTH + (i + mousePosX);
			*(VESA_ADDR + offset) = mouseDeskOri[i][j];
                        
		}
	}
}

void drawMouse(int posX, int posY){      
	
	int i, j;
        unsigned short color;
	unsigned int offset;
	int alpha;

	cleanMouse();

	for (i = 0; i < CURSOR_WIDTH; i++){
		for (j = 0; j < CURSOR_HEIGHT; j++){
			alpha = cursor[i][j] / 65536;
			offset = (j + posY) * SCREEN_WIDTH + (i + posX);
			mouseDeskOri[i][j] = *(VESA_ADDR + offset);
			if(alpha == 0)	continue;

			color = cursor[i][j] % 65536;
			mouseDeskOri[i][j] = *(VESA_ADDR + offset);
			*(VESA_ADDR + offset) =  color;
			
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
	int i, j, alpha;
    unsigned short a, b;
	unsigned int mid; 

	for (i = 0; i < ICON_WIDTH; i++){
		for (j = 0; j < ICON_HEIGHT; j++){
			b = SCREEN_COLOR[posX + i][posY + j];
			a = icon[id][i][j] % 65536;
			alpha = (icon[id][i][j] / 65536) / 8;
			
			mid = (((((((a << 16) | a) & 0x7e0f81f) - (((b << 16) | b) & 0x7e0f81f)) * alpha) >> 5) + (((b << 16) | b) & 0x7e0f81f)) & 0x7e0f81f;
			a = (mid & 0xffff0000) >> 16;
			b = mid & 0xffff;
			SCREEN_COLOR[posX + i][posY + j] =  a | b;
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

int drawWord(int id, int posX, int posY, unsigned short color){
	id -= 33;
	int i, j;
	for (i = 0; i < WORD_HEIGHT; i++){
		for (j = 0; j < WORD_WIDTH; j++){
			if (WORD[id][i][j] == 1)
				SCREEN_COLOR[posX + j][posY + i] = color;
		}
	}
	return WORD_GAP;
}

void drawString(const char* s, int posX, int posY, unsigned short color){
	int len = strlen(s);
	int i;
	int x = posX, y = posY + ICON_HEIGHT + 5;
	for(i = 0; i < len; i++)
	{
		x += drawWord(s[i], x, y, color);
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
	drawString("computer", 0.5*ICON_X1, ICON_Y1, 0xffff);
	drawIcon(1, ICON_X1, ICON_Y2);
	drawString("file", ICON_X1+5, ICON_Y2, 0xffff);
	drawIcon(2, ICON_X1, ICON_Y3);
	drawString("text", ICON_X1+5, ICON_Y3, 0xffff);
	drawIcon(3, ICON_X1, ICON_Y4);
	drawString("paint", ICON_X1, ICON_Y4, 0xffff);
	drawIcon(4, ICON_X1, ICON_Y5);
	drawString("trash", ICON_X1, ICON_Y5, 0xffff);
	//drawIcon(6, ICON_X2, ICON_Y1);
	
	renderScreen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    int i, j;
	for (i = 0; i < CURSOR_WIDTH; i++){
		for (j = 0; j < CURSOR_HEIGHT; j++){
			uint offset = (j + mousePosY) * SCREEN_WIDTH + (i + mousePosX);
			mouseDeskOri[i][j] = *(VESA_ADDR + offset);
		}
	}
	drawMouse(mousePosX,mousePosY);
}

/**************************************************************************
**  folder相关
**  author: yangyanzhe
**  time:   2015.01.21
***************************************************************************/
void
drawFileIcon(int id, int posX, int posY)
{
	int i, j, alpha;
    	unsigned short a, b;
	uint mid; 

	for (i = 0; i < FILE_ICON_SIZE; i++){
		for (j = 0; j < FILE_ICON_SIZE; j++){
			b = SCREEN_COLOR[posX + i][posY + j];
			
			switch(id)
			{
				case 0: a = file_folder[i][j] % 65536;
					alpha = (file_folder[i][j] / 65536) / 8;
					break;
				case 1: a = file_txt[i][j] % 65536;
					alpha = (file_txt[i][j] / 65536) / 8;
					break;
				case 2: a = file_png[i][j] % 65536;
					alpha = (file_png[i][j] / 65536) / 8;
					break;
				case 3: a = file_video[i][j] % 65536;
					alpha = (file_video[i][j] / 65536) / 8;
					break;
				case 4: a = file_exe[i][j] % 65536;
					alpha = (file_exe[i][j] / 65536) / 8;
					break;
				default: cprintf("unknown file type. \n ");
					return;
			}
	
			mid = (((((((a << 16) | a) & 0x7e0f81f) - (((b << 16) | b) & 0x7e0f81f)) * alpha) >> 5) + (((b << 16) | b) & 0x7e0f81f)) & 0x7e0f81f;
			a = (mid & 0xffff0000) >> 16;
			b = mid & 0xffff;
			SCREEN_COLOR[posX + i][posY + j] =  a | b;
		} 
	}
}


void
drawChoosedFileIcon(int id, int posX, int posY)
{
	int i, j, alpha;
    	unsigned short a, b;
	uint mid; 

	for (i = 0; i < FILE_ICON2_SIZE; i++){
		for (j = 0; j < FILE_ICON2_SIZE; j++){
			b = SCREEN_COLOR[posX + i][posY + j];
			
			switch(id)
			{
				case 0: a = file_folder2[i][j] % 65536;
					alpha = (file_folder2[i][j] / 65536) / 8;
					break;
				case 1: a = file_txt2[i][j] % 65536;
					alpha = (file_txt2[i][j] / 65536) / 8;
					break;
				case 2: a = file_png2[i][j] % 65536;
					alpha = (file_png2[i][j] / 65536) / 8;
					break;
				case 3: a = file_video2[i][j] % 65536;
					alpha = (file_video2[i][j] / 65536) / 8;
					break;
				case 4: a = file_exe2[i][j] % 65536;
					alpha = (file_exe2[i][j] / 65536) / 8;
					break;
				default: cprintf("unknown file type. \n ");
					return;
			}
	
			mid = (((((((a << 16) | a) & 0x7e0f81f) - (((b << 16) | b) & 0x7e0f81f)) * alpha) >> 5) + (((b << 16) | b) & 0x7e0f81f)) & 0x7e0f81f;
			a = (mid & 0xffff0000) >> 16;
			b = mid & 0xffff;
			SCREEN_COLOR[posX + i][posY + j] =  a | b;
		} 
	}
}


