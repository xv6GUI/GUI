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
#define LINE_HEIGHT 20
#define HEIGHT_BEGIN 20
#define WORD_BEGIN 23
#define WIDTH_BEGIN 15

char text[MAX_LENGTH];
int text_word_width[MAX_LENGTH];
unsigned int x_start, y_start;
int cursor;
int text_length = -1;
unsigned int cursor_x, cursor_y;
int flag_out;		// ok, 0; width or height beyond, 1;

void
draw_text_back()
{
	int i, j;
	for(i = 1; i < WindowWidth - 1; i++)
	{
		for(j = HEIGHT_BEGIN; j < WindowHeight - HEIGHT_BEGIN; j++)
		{
			unsigned short color;
			if(j % LINE_HEIGHT == 2 * LINE_HEIGHT - WORD_BEGIN)
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
	for(i = y; i < y + 2 * LINE_HEIGHT - WORD_BEGIN - 1; i++)
		draw_point(x, i, 0);
}

void
draw_text_string()
{
	int i, line_len, height_len;
	i = 0;
	line_len = x_start + WIDTH_BEGIN;
	height_len = y_start + WORD_BEGIN;
	if(cursor == 0)
		draw_cursor(line_len, height_len);
	while(i < MAX_LENGTH && text[i] != '\0')
	{
		if(line_len > WindowWidth || text[i] == '\n')
		{
			height_len += LINE_HEIGHT;
			line_len = x_start + WIDTH_BEGIN;
		}
		text_word_width[i] = draw_character(line_len, height_len, text[i], 0);
		line_len += text_word_width[i];
		if(i == cursor - 1)
		{
			draw_cursor(line_len, height_len);
		}
		i++;
	}
	if(line_len > WindowWidth && height_len > WindowHeight - HEIGHT_BEGIN)
		flag_out = 1;
	else if(height_len > WindowHeight - HEIGHT_BEGIN)
		flag_out = 2;
	else
		flag_out = 0;
}

void 
init_text(unsigned int x, unsigned int y)
{
	x_start = x;
	y_start = y;
	if(text_length == -1)
	{
		text[0] = '\0';
		cursor = 0;
		text_length = 0;
		cursor_x = x_start + WIDTH_BEGIN;
		cursor_y = y_start + WORD_BEGIN;
		flag_out = 0;
	}

	draw_text_back();
	draw_text_string();
}

void
add_char(char ch)
{
	if(text_length >= MAX_LENGTH - 1 || flag_out == 1)
		return;
	if(flag_out == 2 && ch == '\n')
		return;

	int i;
	for(i = text_length; i >= cursor; i--)
	{
		text[i + 1] = text[i];
	}
	text[cursor] = ch;
	cursor++;
	text_length++;
}

void
delete_char()
{
	if(cursor == 0)
		return;

	int i;
	for(i = cursor - 1; i < text_length; i++)
	{
		text[i] = text[i + 1];
	}
	cursor--;
	text_length--;
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
		if(cursor != text_length)
			cursor++;
	}
	else if (flag == KBD_UP)
	{
		int i = cursor - 1;
		int len = WIDTH_BEGIN;
		while(1)
		{
			len += text_word_width[i];
			if(len > WindowWidth || text[i] == '\0')
			{
				cursor = i + 1;
				break;
			}
			else if(text[i] == '\n')
			{
				cursor = i;
				break;
			}
			i--;
		}
		
	}
	else if (flag == KBD_DOWN)
	{
		int i = cursor + 1;
		int len = WIDTH_BEGIN;
		while(1)
		{
			len += text_word_width[i];
			if(len > WindowWidth || i == text_length || text[i] == '\n')
			{
				cursor = i;
				break;
			}
			i++;
		}		
	}
	else if(flag == KBD_END)
	{
		cursor = text_length;
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