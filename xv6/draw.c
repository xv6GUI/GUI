#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
#include "gui.h"
#include "window.h"

#define COLOR_BLACK 0x0000
#define COLOR_RED 0xF800
#define COLOR_GREEN 0x07E0
#define COLOR_BLUE 0x001F
#define COLOR_YELLOW 0xFFE0
#define COLOR_CYAN	0x07FF
#define COLOR_MEGANTA 0xF81F
#define COLOR_WHITE 0xFFFF

#define black 0
#define red 1
#define green 2
#define blue 3
#define yellow 4
#define cyan 5
#define meganta 6
#define white 7

unsigned short canvas[CANVAS_WIDTH][CANVAS_HEIGHT];
unsigned short current_color = COLOR_BLACK;
unsigned short *ADDR = (unsigned short*)0xe0000000;
int init_flag = 0;

int circle[10][10] = {
	{0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
	{0, 0, 1, 2, 2, 2, 2, 1, 0, 0},
	{0, 1, 2, 2, 2, 2, 2, 2, 1, 0},
	{1, 2, 2, 2, 2, 2, 2, 2, 2, 1},
	{1, 2, 2, 2, 2, 2, 2, 2, 2, 1},
	{1, 2, 2, 2, 2, 2, 2, 2, 2, 1},
	{1, 2, 2, 2, 2, 2, 2, 2, 2, 1},
	{0, 1, 2, 2, 2, 2, 2, 2, 1, 0},
	{0, 0, 1, 2, 2, 2, 2, 1, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 0, 0, 0}
};

int eraser[16][16] = {
	{65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 61309, 65503, 65535, 63422, 65535},
	{65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 61277, 59196, 54938, 65471, 54970, 63422},
	{65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 60854, 52630, 61309, 59196, 52857, 46518, 57083},
	{65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 60854, 51492, 47331, 54905, 61244, 59164, 44406, 50712},
	{65535, 65535, 65535, 65535, 65535, 65535, 65535, 60887, 51557, 49379, 56531, 54938, 52793, 59164, 52857, 46485},
	{65535, 65535, 65535, 65535, 65535, 65535, 62967, 51622, 49476, 54418, 54710, 54483, 45186, 44243, 57083, 46486},
	{65535, 65535, 65535, 65535, 65535, 62999, 53768, 51590, 56466, 52500, 54385, 49185, 47137, 52435, 65535, 48598},
	{65535, 65535, 65535, 65535, 63032, 55913, 53703, 56596, 54645, 54385, 49184, 47137, 56596, 63390, 65535, 48631},
	{65535, 65535, 65535, 63032, 58026, 55848, 58676, 54678, 56434, 51265, 49184, 58676, 65535, 63422, 65535, 50712},
	{65535, 65535, 63065, 58123, 57993, 56563, 54613, 58644, 53345, 51264, 58676, 65535, 65535, 65503, 65535, 52857},
	{65535, 65535, 46323, 58188, 60887, 56758, 58578, 55490, 51264, 58676, 65535, 65535, 65535, 65535, 63455, 54970},
	{65535, 65535, 52825, 46420, 61309, 60821, 55555, 55393, 60724, 65535, 65535, 65535, 65535, 65535, 63390, 59164},
	{65535, 65535, 65535, 50744, 46420, 57701, 57635, 60789, 65535, 65535, 65535, 65535, 65535, 65535, 61342, 61277},
	{65535, 65535, 65535, 65535, 52825, 54548, 62870, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 63358, 63390},
	{65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 63390, 65503},
	{65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535}
};

int reset[16][16] = {
	{65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535},
	{65535, 65535, 65535, 65535, 59359, 40733, 28381, 20125, 28381, 44894, 65535, 65535, 65535, 65535, 65535, 65535},
	{65535, 65535, 65535, 38686, 18045, 15997, 7805, 1628, 1596, 1532, 9788, 55199, 65503, 63423, 65503, 65535},
	{65535, 65535, 34525, 22173, 20126, 14014, 3743, 1662, 1598, 1565, 1532, 3548, 51005, 59196, 61309, 65503},
	{65535, 44862, 24254, 24287, 20190, 13591, 10695, 10760, 5172, 1598, 1533, 1500, 5595, 48664, 57050, 63422},
	{65535, 26269, 26335, 24319, 20158, 10695, 16903, 16904, 14823, 6958, 1565, 1533, 1500, 21749, 48631, 61277},
	{53150, 26302, 26367, 24319, 17686, 14823, 15214, 3612, 3482, 9005, 3124, 1533, 1468, 7447, 42260, 59163},
	{48990, 28350, 26367, 24319, 15377, 16904, 7642, 1662, 1630, 1598, 978, 1468, 1500, 3385, 40179, 57051},
	{48990, 28350, 26334, 15313, 14986, 19016, 15312, 3547, 1630, 1597, 1565, 1435, 1500, 3417, 42227, 57051},
	{53150, 26302, 26367, 22206, 14889, 16937, 9560, 1662, 1629, 1598, 1565, 1533, 1468, 11576, 44405, 59164},
	{65535, 28317, 26334, 24319, 17653, 15117, 3742, 1662, 1630, 1598, 1565, 1533, 1468, 27991, 50744, 61309},
	{65535, 48990, 24254, 22206, 20159, 13591, 5790, 1662, 1629, 1597, 1565, 1500, 11771, 52889, 59164, 65470},
	{65535, 65535, 38653, 22173, 20126, 14014, 3742, 1662, 1598, 1565, 1532, 7676, 55166, 61277, 63390, 65535},
	{65535, 65535, 65535, 44862, 20093, 13949, 7774, 1597, 1596, 1532, 20093, 59359, 65503, 65503, 65535, 65535},
	{65535, 65535, 65535, 65535, 63455, 44862, 34557, 28381, 34589, 55198, 65535, 65535, 65535, 65535, 65535, 65535},
	{65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535}
};

void
set_brush_color(unsigned int x)
{
	if(x > 30 && x < 50)
		current_color = COLOR_BLACK;
	else if(x > 55 && x < 75)
		current_color = COLOR_RED;
	else if(x > 80 && x < 100)
		current_color = COLOR_GREEN;
	else if(x > 105 && x < 125)
		current_color = COLOR_BLUE;
	else if(x > 130 && x < 150)
		current_color = COLOR_YELLOW;
	else if(x > 155 && x < 175)
		current_color = COLOR_CYAN;
	else if(x > 180 && x < 200)
		current_color = COLOR_MEGANTA;
	else if(x > 270 && x < 295)
		current_color = COLOR_WHITE;
}

void
draw_brush(unsigned int x, unsigned int y)
{
  int i, j;
  unsigned short color = current_color;
  if(color == COLOR_WHITE)
  	color = COLOR_BLACK;
  for(i = 0; i < 10; i++)
  {
    for(j = 0; j < 10; j++)
    {
      if(circle[i][j] == 1)
      {
        unsigned int offset = (j + y) * SCREEN_WIDTH + (i + x);
        *(ADDR + offset) = color;
      }
    }
  }
}

void
clean_canvas(struct Window* window)
{
	int i, j;
	for(i = 0; i < CANVAS_WIDTH; ++i)
	{
		for(j = 0; j < CANVAS_HEIGHT; ++j)
		{
			canvas[i][j] = COLOR_WHITE;
		}
	}
	int x, y;
	x = 30 + window->Pos_x;
	y = 70 + window->Pos_y;
	for(i = 0; i < CANVAS_WIDTH; i++)
	{
		for(j = 0; j < CANVAS_HEIGHT; j++)
		{
			draw_point(x + i, y + j, COLOR_WHITE);
		}
	}
	display_to_screen(x, y, CANVAS_WIDTH, CANVAS_HEIGHT);
}

int 
is_canvans(unsigned int x, unsigned int y)
{
	if(x >= 30 && x <= 20 + CANVAS_WIDTH
		&& y >= 70 && y <= 60 + CANVAS_HEIGHT)
		return 1;
	else
		return 0;
}

void
draw(struct Window* window, unsigned int x, unsigned int y, int event)
{
	if(event == LEFT_CLICK || event == DOUBLE_CLICK)
	{
		if(y > 40 && y < 60)
		{
			if(x < 300)
				set_brush_color(x);
			else if(x > 300 && x < 320)
				clean_canvas(window);
			return;
		}
	}

	if(event == DRAGGING || event == LEFT_CLICK)
	{
		if(is_canvans(x, y))
		{
			int i, j;
			int draw_x = x + window->Pos_x;
			int draw_y = y + window->Pos_y;
			x -= 30;
			y -= 70;
			for(i = 0; i < 10; i++)
			{
				for(j = 0; j < 10; j++)
				{
				  if(circle[i][j] != 0)
				  {
				    canvas[x + i][y + j] = current_color;
				    draw_point(i + draw_x, j + draw_y, current_color);
				  }
				}
			}	
		}
	}
}

void
init_draw(unsigned int x, unsigned int y)
{
	int i, j, id;
	int offset_x, offset_y;

	if(init_flag == 0)
	{
		for(i = 0; i < CANVAS_WIDTH; i++)
		{
			for(j = 0; j < CANVAS_HEIGHT; j++)
			{
				canvas[i][j] = COLOR_WHITE;
			}
		}
		init_flag = 1;
	}

	offset_x = x + 1;
	offset_y = y + 20;
	for(i = 0; i < WindowWidth - 2; ++i)
	{
		for(j = 0; j < WindowHeight - 40; ++j)
		{
			draw_point(offset_x + i, offset_y + j, 0x953C);
		}
	}

	offset_x = x + 30;
	offset_y = y + 40;
	for(id = 0; id < 7; id++)
	{
		for(i = 0; i < 20; i++)
		{
			for(j = 0; j < 20; j++)
			{
				if(i == 0 || j == 0 || i == 19 || j == 19)
					draw_point(i + offset_x, j + offset_y, COLOR_BLACK);
				else
				{
					switch(id)
					{
						case black: 
							draw_point(i + offset_x, j + offset_y, COLOR_BLACK); 
							break;
						case red: 
							draw_point(i + offset_x, j + offset_y, COLOR_RED); 
							break;
						case green:
							draw_point(i + offset_x, j + offset_y, COLOR_GREEN); 
							break;
						case blue: 
							draw_point(i + offset_x, j + offset_y, COLOR_BLUE); 
							break;
						case yellow: 
							draw_point(i + offset_x, j + offset_y, COLOR_YELLOW); 
							break;
						case cyan: 
							draw_point(i + offset_x, j + offset_y, COLOR_CYAN); 
							break;
						case meganta: 
							draw_point(i + offset_x, j + offset_y, COLOR_MEGANTA); 
							break;
					}
				}
			}
		}
		offset_x += 25;
	}

	offset_x += 70;
	int k;
	for(k = 0; k < 2; k++)
	{
		for(i = 0; i < 20; i++)
		{
			for(j = 0; j < 20; j++)
			{
				if(i == 0 || j == 0 || i == 19 || j == 19)
					draw_point(i + offset_x, j + offset_y, COLOR_BLACK);
				else
					draw_point(i + offset_x, j + offset_y, COLOR_WHITE);
			}
		}
		offset_x += 25;
	}
	offset_x -= 50;
	offset_x += 2;
	offset_y += 2;
	for(i = 0; i < 16; i++)
	{
		for(j = 0; j < 16; j++)
		{
			draw_point(i + offset_x, j + offset_y, eraser[i][j]);
		}
	}
	offset_x += 25;
	for(i = 0; i < 16; i++)
	{
		for(j = 0; j < 16; j++)
		{
			draw_point(i + offset_x, j + offset_y, reset[i][j]);
		}
	}

	offset_x = x + 30;
	offset_y = y + 70;
	for(i = 0; i < CANVAS_WIDTH; i++)
	{
		for(j = 0; j < CANVAS_HEIGHT; j++)
		{
			draw_point(i + offset_x, j + offset_y, canvas[i][j]);
		}
	}
}
