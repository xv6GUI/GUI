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
#include "PHOTO.h"

#define PHOTO_NUM 8
#define ANIMATION_NUM 8

int count = 0;
int animation_count = 0;

//left to right
void
animation1()
{
	int i, j;
	for(i = 0; i < SCREEN_WIDTH; i++)
	{
		for(j = 0; j < SCREEN_HEIGHT; j++)
		{
			draw_background_point(i, j, count);
		}
		if(i % 20 == 19)
		{
			display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
}

//right to left
void
animation2()
{
	int i, j;
	for(i = 0; i < SCREEN_WIDTH; i++)
	{
		for(j = 0; j < SCREEN_HEIGHT; j++)
		{
			draw_background_point(SCREEN_WIDTH - 1 - i, j, count);
		}
		if(i % 20 == 19)
		{
			display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
}

//top to down
void
animation3()
{
	int i, j;
	for(i = 0; i < SCREEN_HEIGHT; i++)
	{
		for(j = 0; j < SCREEN_WIDTH; j++)
		{
			draw_background_point(j, i, count);
		}
		if(i % 20 == 19)
		{
			display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
}

//down to top
void
animation4()
{
	int i, j;
	for(i = 0; i < SCREEN_HEIGHT; i++)
	{
		for(j = 0; j < SCREEN_WIDTH; j++)
		{
			draw_background_point(j, SCREEN_HEIGHT - 1 - i, count);
		}
		if(i % 20 == 19)
		{
			display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
}

//middle to right & left
void
animation5()
{
	int i, j;
	for(i = 0; i < SCREEN_WIDTH / 2; i++)
	{
		for(j = 0; j < SCREEN_HEIGHT; j++)
		{
			draw_background_point(SCREEN_WIDTH / 2 + i, j, count);
			draw_background_point(SCREEN_WIDTH / 2 - i, j, count);
		}
		if(i % 10 == 9)
		{
			display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
}

//right & left to middle
void
animation6()
{
	int i, j;
	for(i = 0; i < SCREEN_WIDTH / 2; i++)
	{
		for(j = 0; j < SCREEN_HEIGHT; j++)
		{
			draw_background_point(i, j, count);
			draw_background_point(SCREEN_WIDTH - 1 - i, j, count);
		}
		if(i % 10 == 9)
		{
			display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
}

//middle to top & down
void
animation7()
{
	int i, j;
	for(i = 0; i < SCREEN_HEIGHT / 2; i++)
	{
		for(j = 0; j < SCREEN_WIDTH; j++)
		{
			draw_background_point(j, SCREEN_HEIGHT / 2 + i, count);
			draw_background_point(j, SCREEN_HEIGHT / 2 - i, count);
		}
		if(i % 10 == 9)
		{
			display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
}


//top & down to middle
void
animation8()
{
	int i, j;
	for(i = 0; i < SCREEN_HEIGHT / 2; i++)
	{
		for(j = 0; j < SCREEN_WIDTH; j++)
		{
			draw_background_point(j, i, count);
			draw_background_point(j, SCREEN_HEIGHT  - 1- i, count);
		}
		if(i % 10 == 9)
		{
			display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
}

//left-right seperate
void
animation9()
{
	int i, j;
	for(i = 0; i < SCREEN_WIDTH; i++)
	{
		for(j = 0; j < SCREEN_HEIGHT; j++)
		{
			draw_background_point(i, j, count);
		}
		if(i % 20 == 19)
		{
			display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			i += 20;
		}
	}
	for(i = 0; i < SCREEN_WIDTH; i++)
	{
		for(j = 0; j < SCREEN_HEIGHT; j++)
		{
			draw_background_point(SCREEN_WIDTH - 1 - i, j, count);
		}
		if(i % 20 == 19)
		{
			display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			i += 20;
		}
	}
}

//top-down seperate
void
animation10()
{
	int i, j;
	for(i = 0; i < SCREEN_HEIGHT; i++)
	{
		for(j = 0; j < SCREEN_WIDTH; j++)
		{
			draw_background_point(j, i, count);
		}
		if(i % 20 == 19)
		{
			display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			i += 20;
		}
	}
	for(i = 0; i < SCREEN_HEIGHT; i++)
	{
		for(j = 0; j < SCREEN_WIDTH; j++)
		{
			draw_background_point(j, SCREEN_HEIGHT - 1 - i, count);
		}
		if(i % 20 == 19)
		{
			display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			i += 20;
		}
	}
}

//center to edge
void
animation11()
{
	int i, j, k;
	for(i = 0; i < 21; i++)
	{
		for(j = SCREEN_WIDTH / 2 - i * 20; j < SCREEN_WIDTH / 2 - (i - 1) * 20; j++)
		{
			for(k = SCREEN_HEIGHT / 2 - i * 15; k < SCREEN_HEIGHT / 2 + i * 15; k++)
			{
				draw_background_point(j, k, count);
			}
		}
		for(j = SCREEN_WIDTH / 2 + (i - 1) * 20; j < SCREEN_WIDTH / 2 + i * 20; j++)
		{
			for(k = SCREEN_HEIGHT / 2 - i * 15; k < SCREEN_HEIGHT / 2 + i * 15; k++)
			{
				draw_background_point(j, k, count);
			}
		}
		for(j = SCREEN_WIDTH / 2 - (i - 1) * 20; j < SCREEN_WIDTH / 2 + (i - 1) * 20; j++)
		{
			for(k = SCREEN_HEIGHT / 2 - i * 15; k < SCREEN_HEIGHT / 2 - (i - 1) * 15; k++)
			{
				draw_background_point(j, k, count);
			}
		}
		for(j = SCREEN_WIDTH / 2 - (i - 1) * 20; j < SCREEN_WIDTH / 2 + (i - 1) * 20; j++)
		{
			for(k = SCREEN_HEIGHT / 2 + (i - 1) * 15; k < SCREEN_HEIGHT / 2 + i * 15; k++)
			{
				draw_background_point(j, k, count);
			}
		}
		display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}

//edge to center
void
animation12()
{
	int i, j, k;
	for(i = 20; i >= 0; i--)
	{
		for(j = SCREEN_WIDTH / 2 - i * 20; j < SCREEN_WIDTH / 2 - (i - 1) * 20; j++)
		{
			for(k = SCREEN_HEIGHT / 2 - i * 15; k < SCREEN_HEIGHT / 2 + i * 15; k++)
			{
				draw_background_point(j, k, count);
			}
		}
		for(j = SCREEN_WIDTH / 2 + (i - 1) * 20; j < SCREEN_WIDTH / 2 + i * 20; j++)
		{
			for(k = SCREEN_HEIGHT / 2 - i * 15; k < SCREEN_HEIGHT / 2 + i * 15; k++)
			{
				draw_background_point(j, k, count);
			}
		}
		for(j = SCREEN_WIDTH / 2 - (i - 1) * 20; j < SCREEN_WIDTH / 2 + (i - 1) * 20; j++)
		{
			for(k = SCREEN_HEIGHT / 2 - i * 15; k < SCREEN_HEIGHT / 2 - (i - 1) * 15; k++)
			{
				draw_background_point(j, k, count);
			}
		}
		for(j = SCREEN_WIDTH / 2 - (i - 1) * 20; j < SCREEN_WIDTH / 2 + (i - 1) * 20; j++)
		{
			for(k = SCREEN_HEIGHT / 2 + (i - 1) * 15; k < SCREEN_HEIGHT / 2 + i * 15; k++)
			{
				draw_background_point(j, k, count);
			}
		}
		display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}

//cell
void
animation13()
{
	int i, j, u, v;
	for(i = 0; i < 20; i += 2)
	{
		for(j = 1; j < 20; j += 2)
		{
			for(u = i * 40; u < (i + 1) * 40; u++)
			{
				for(v = j * 30; v < (j + 1) * 30; v++)
				{
					draw_background_point(u, v, count);
				}
			}
		}
		display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	for(i = 1; i < 20; i += 2)
	{
		for(j = 0; j < 20; j += 2)
		{
			for(u = i * 40; u < (i + 1) * 40; u++)
			{
				for(v = j * 30; v < (j + 1) * 30; v++)
				{
					draw_background_point(u, v, count);
				}
			}
		}
		display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	for(i = 0; i < 20; i += 2)
	{
		for(j = 0; j < 20; j += 2)
		{
			for(u = i * 40; u < (i + 1) * 40; u++)
			{
				for(v = j * 30; v < (j + 1) * 30; v++)
				{
					draw_background_point(u, v, count);
				}
			}
		}
		display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	for(i = 1; i < 20; i += 2)
	{
		for(j = 1; j < 20; j += 2)
		{
			for(u = i * 40; u < (i + 1) * 40; u++)
			{
				for(v = j * 30; v < (j + 1) * 30; v++)
				{
					draw_background_point(u, v, count);
				}
			}
		}
		display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}

//to black
void
the_end()
{
	int i, j, k;
	for(i = 20; i >= 0; i--)
	{
		for(j = SCREEN_WIDTH / 2 - i * 20; j < SCREEN_WIDTH / 2 - (i - 1) * 20; j++)
		{
			for(k = SCREEN_HEIGHT / 2 - i * 15; k < SCREEN_HEIGHT / 2 + i * 15; k++)
			{
				draw_point(j, k, 0);
			}
		}
		for(j = SCREEN_WIDTH / 2 + (i - 1) * 20; j < SCREEN_WIDTH / 2 + i * 20; j++)
		{
			for(k = SCREEN_HEIGHT / 2 - i * 15; k < SCREEN_HEIGHT / 2 + i * 15; k++)
			{
				draw_point(j, k, 0);
			}
		}
		for(j = SCREEN_WIDTH / 2 - (i - 1) * 20; j < SCREEN_WIDTH / 2 + (i - 1) * 20; j++)
		{
			for(k = SCREEN_HEIGHT / 2 - i * 15; k < SCREEN_HEIGHT / 2 - (i - 1) * 15; k++)
			{
				draw_point(j, k, 0);
			}
		}
		for(j = SCREEN_WIDTH / 2 - (i - 1) * 20; j < SCREEN_WIDTH / 2 + (i - 1) * 20; j++)
		{
			for(k = SCREEN_HEIGHT / 2 + (i - 1) * 15; k < SCREEN_HEIGHT / 2 + i * 15; k++)
			{
				draw_point(j, k, 0);
			}
		}
		display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}

void
next_photo()
{
	//cprintf("count: %d\n", count);
	if(count == PHOTO_NUM)
	{
		the_end();
		return;
	}
	switch(animation_count)
	{
		case 0:
			animation1();
			break;
		case 1:
			animation2();
			break;
		//case 2:
			//animation3();
			//break;
		//case 3:
			//animation4();
			//break;
		case 2:
			animation5();
			break;
		case 3:
			animation6();
			break;
		//case 6:
			//animation7();
			//break;
		//case 7:
			//animation8();
			//break;
		case 4:
			animation9();
			break;
		case 5:
			animation10();
			break;
		case 6:
			animation11();
			break;
		//case 11:
			//animation12();
			//break;
		case 7:
			animation13();
			break;
	}
	animation_count = (animation_count + 1) % ANIMATION_NUM;
}

void
pre_photo()
{
	int i, j;
	for(i = 0; i < SCREEN_WIDTH; i++)
	{
		for(j = 0; j < SCREEN_HEIGHT; j++)
		{
			draw_background_point(i, j, count - 1);
		}
	}
	display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void
photo(char kbd_ch, int kbd_flag)
{
	if(count == 0)
	{
		if(kbd_ch == ' ')
		{
			next_photo();
			count++;
		}
	}
	else if(kbd_flag == KBD_ESC)
	{
		draw_again(get_current_bg());
		count = 0;
	}
	else if(kbd_flag == KBD_RIGHT)
	{
		if(count > PHOTO_NUM)
		{
			draw_again(get_current_bg());
			//count = 0;
			return;
		}
		next_photo();
		count++;
	}
	else if(kbd_flag == KBD_LEFT)
	{
		if(count > 1)
		{
			count--;
			pre_photo();
		}
	}
}

void 
init_photo(unsigned int x, unsigned int y)
{
	int i, j;
	for(i = 0; i < WindowWidth - 2; i++)
	{
		for(j = 0; j < WindowHeight - 40; j++)
		{
			draw_point(x + i + 1, y + j + 20, back[i][j]);
		}
	}
}