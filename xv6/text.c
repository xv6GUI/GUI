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

#define MAX_LENGTH 2000		// num of word list
#define LINE_HEIGHT 25		// height of each line
#define WIDTH_BEGIN 25		// width between side and line
#define HEIGHT_BEGIN 40		// height between top and word
#define LINE 530		// width of line
#define WORD_HEIGHT 20 		// height of each word

char text[MAX_LENGTH];
int text_word_width[MAX_LENGTH];
unsigned int x_start, y_start;
int cursor;
int text_length = -1;
unsigned int cursor_x, cursor_y;
int full;		// ok, 0; width or height beyond, 1;

void
drawCursor(unsigned int x, unsigned int y)
{
	int i;
	for(i = y; i < y + WORD_HEIGHT; i++)
		drawPoint(x, i, 0x0000);
}

void
drawText()
{
	int i, line_len, height_len;
	i = 0;
	line_len = x_start + WIDTH_BEGIN;
	height_len = y_start + HEIGHT_BEGIN;
	if(cursor == 0)
		drawCursor(line_len, height_len);
	while(i < MAX_LENGTH && text[i] != '\0')
	{
		if(line_len > LINE + WIDTH_BEGIN || text[i] == '\n')
		{
			height_len += LINE_HEIGHT;
			line_len = x_start + WIDTH_BEGIN;
		}
		text_word_width[i] = drawWord(text[i], line_len, height_len, 0x0000);
		line_len += text_word_width[i];
		if(i == cursor - 1)
		{
			drawCursor(line_len, height_len);
		}
		i++;
	}
	if(height_len > WINDOW_HEIGHT - HEIGHT_BEGIN)
		full = 1;
	else
		full = 0;
}

void 
initText(unsigned int x, unsigned int y)
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
		full = 0;
	}

	drawText();
}

void
addWord(char ch)
{
	if(text_length >= MAX_LENGTH - 1 || (full == 1 && ch == '\n')
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
deleteWord()
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
		int len = 0;
		while(1)
		{
			len += text_word_width[i];
			if(len > LINE || text[i] == '\0')
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
		int len = 0;
		while(1)
		{
			len += text_word_width[i];
			if(len > LINE || i == text_length || text[i] == '\n')
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
		addWord(ch);
	else if(ch == '\b')
		deleteWord();
	drawText();
	renderScreen(x_start, y_start, LINE, WindowHeight);
}
