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
#include "THUMBNAIL.h"

#define THUMBNAIL_WIDTH 160
#define THUMBNAIL_HEIGHT 120

int current_bg_id = 0;;

int 
get_id_by_point(unsigned int x, unsigned int y)
{
	int line = -1, col = -1;
	int offset_x = 1, offset_y = 20;
	if(x > 30 + offset_x && x < 190  + offset_x)
		col = 0;
	else if(x > 220 + offset_x && x < 380 + offset_x)
		col = 1;
	else if(x > 410 + offset_x && x < 570 + offset_x)
		col = 2;

	if(y > 30 + offset_y && y < 150 + offset_y)
		line = 0;
	else if(y > 180 + offset_y && y < 300 + offset_y)
		line = 1;

	return (col + line * 3);
}

int 
get_current_bg()
{
	return current_bg_id;
}

int
set_current_bg(int id)
{
	if(id >= 0 && id < 5)
	{
		current_bg_id = id;
		return 1;
	}
	else
		return 0;
}

void
setting(unsigned int x, unsigned int y, int mouse_event)
{
	if(mouse_event != LEFT_CLICK)
		return;

	int id = get_id_by_point(x, y);
	if(id >= 0 && id < 5)
	{
		set_current_bg(id);
		draw_again(current_bg_id);
	}
}

void 
draw_thumbnail(unsigned int x, unsigned int y, int n)
{
	int i, j;
	for(i = 0; i < THUMBNAIL_WIDTH; ++i)
	{
		for (j = 0; j < THUMBNAIL_HEIGHT; ++j)
		{
			if(i == 0 || j == 0 || i == THUMBNAIL_WIDTH - 1 || j == THUMBNAIL_HEIGHT - 1)
				draw_point(x + i, y + j, 0);
			else
				draw_point(x + i, y + j, thumbnail[n][i][j]);
		}
	}
}

void
init_setting(unsigned int x, unsigned int y)
{
	int i, j;
	int offset_x, offset_y;
	x += 1;
	y += 20;
	for(i = 0; i < WindowWidth - 2; ++i)
	{
		for(j = 0; j < WindowHeight - 40; ++j)
		{
			draw_point(x + i, y + j, 0xFFFF);
		}
	}
	for(i = 0; i < 5; i++)
	{	
		offset_x = 30 * (i % 3 + 1) + 160 * (i % 3);
		offset_y = 30 * (i / 3 + 1) + 120 * (i / 3);
		draw_thumbnail(x + offset_x, y + offset_y, i);
	}
}