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

#define MAX_LENGTH 2000

char text_string[MAX_LENGTH];
int text_string_per_width[MAX_LENGTH];
unsigned int x_start, y_start;
int cursor;
int string_length = -1;
unsigned int cursor_x, cursor_y;
int add_lock;

void
draw_text_back()
{
	int i, j;
	for(i = 1; i < WindowWidth - 1; i++)
	{
		for(j = 20; j < WindowHeight - 20; j++)
		{
			unsigned short color;
			if(j % 20 == 17)
				color = 0xD674;
			else
				color = 0xF758;
			draw_point(i + x_start, j + y_start, color);
		}
	}
}

void
draw_cursor(unsigned int x, unsigned int y)
{
	int i;
	//int m = y_start + y;
	//int n = y_start + y + 18;
	//cprintf("start: %d, end: %d\n", m, n);
	for(i = y; i < y + 16; i++)
		draw_point(x, i, 0);
}

void
draw_text_string()
{
	/*
	int m;
	for(m = 0; m < MAX_LENGTH - 1; m++)
	{
		text_string[m] = 'W';
	}
	text_string[MAX_LENGTH - 1] = '\0';
	*/

	int i, j, k;
	i = 0;
	j = x_start + 12;
	k = y_start + 23;
	if(cursor == 0)
		draw_cursor(j, k);
	while(i < MAX_LENGTH && text_string[i] != '\0')
	{
		if(j > WindowWidth || text_string[i] == '\n')
		{
			k += 20;
			j = x_start + 12;
		}
		text_string_per_width[i] = draw_character(j, k, text_string[i], 0);
		j += text_string_per_width[i];
		//cprintf("i: %d, cursor: %d\n", i, cursor);
		if(i == cursor - 1)
		{
			draw_cursor(j, k);
		}
		i++;
	}
	//cprintf("j: %d, k: %d\n", j, k);
	if(j > WindowWidth && k > WindowHeight - 20)
		add_lock = 1;
	else if(k > WindowHeight - 20)
		add_lock = 2;
	else
		add_lock = 0;
}

void 
init_text(unsigned int x, unsigned int y)
{
	x_start = x;
	y_start = y;
	if(string_length == -1)
	{
		text_string[0] = '\0';
		cursor = 0;
		string_length = 0;
		cursor_x = x_start + 12;
		cursor_y = y_start + 23;
		add_lock = 0;
	}

	draw_text_back();
	draw_text_string();
}

void
add_char(char ch)
{
	if(string_length >= MAX_LENGTH - 1 || add_lock == 1)
		return;

	if(add_lock == 2 && ch == '\n')
		return;

	int i;
	//cprintf("length: %d, cursor: %d\n", string_length, cursor);
	for(i = string_length; i >= cursor; i--)
	{
		text_string[i + 1] = text_string[i];
	}
	text_string[cursor] = ch;
	cursor++;
	string_length++;
}

void
delete_char()
{
	if(cursor == 0)
		return;

	int i;
	for(i = cursor - 1; i < string_length; i++)
	{
		text_string[i] = text_string[i + 1];
	}
	cursor--;
	string_length--;
}

void
kbd_text(char ch, int flag)
{
	if(flag == KBD_LEFT)
	{
		if(cursor != 0)
			cursor--;
	}
	else if(flag == KBD_RIGHT)
	{
		if(cursor != string_length)
			cursor++;
	}
	else if(flag < 0)
		return;
	else if((ch >= 0x20 && ch <= 0x7E) || ch == '\n')
		add_char(ch);
	else if(ch == '\b')
		delete_char();
	draw_text_back();
	draw_text_string();
	display_to_screen(x_start, y_start, WindowWidth, WindowHeight);
}