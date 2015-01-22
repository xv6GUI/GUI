#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
#include "gui.h"
#include "draw.h"
#include "mouse.h"

unsigned short canvas[CANVAS_WIDTH][CANVAS_HEIGHT];
unsigned short current_color = COLOR_BLACK;
unsigned short *ADDR = (unsigned short*)0xe0000000;
int command = 0;	// paint command
int line_width = LINE_WIDTH;
int x_start = 0, y_start = 0, x_end = 0, y_end = 0;
int window_x, window_y;

struct text_word word[TEXT_LEN];
int text_id = 0;

int circle[6][6] = {
	{0, 0, 1, 1, 0, 0},
	{0, 1, 2, 2, 1, 0},
	{1, 2, 2, 2, 2, 1},
	{1, 2, 2, 2, 2, 1},
	{0, 1, 2, 2, 1, 0},
	{0, 0, 1, 1, 0, 0}
};

int
max(int a, int b)
{
	return (a > b ? a : b);
}

int
min(int a, int b)
{
	return (a < b ? a : b);
}

void
init(int id, int x_start, int x_end, int y_start, int y_end)
{
	word[id].text[0] = '\0';
	word[id].cursor = 0;
	word[id].text_length = 0;
	word[id].cursor_x = word[id].x_start;
	word[id].cursor_y = word[id].y_start;
	word[id].full = 0;
	word[id].x_start = min(x_start, x_end);
	word[id].width = max(x_start, x_end) - word[text_id].x_start;
	word[id].y_start = min(y_start, y_end);
	word[id].height = max(y_start, y_end) - word[text_id].y_start;
}

void
add_char(char ch, int id)
{
	if(word[id].text_length >= WORD_LEN - 1 || (word[id].full == 1 && (ch == '\n' || ch == '\r')))
		return;
	int i;
	for(i = word[id].text_length; i >= word[id].cursor; i--)
	{
		word[id].text[i+1] = word[id].text[i];
	}
	word[id].text[word[id].cursor] = ch;
	word[id].cursor++;
	word[id].text_length++;
	cprintf("add_char!\n");
}

void
delete_char(int id)
{
	if(word[id].cursor == 0)
		return;
	int i;
	for(i = word[id].cursor - 1; i < word[id].text_length; i++)
	{
		word[id].text[i] = word[id].text[i + 1];
	}
	word[id].cursor--;
	word[id].text_length--;
}

void
draw_cursor(uint x, uint y)
{
	int i;
	for(i = y; i < y + WORD_HEIGHT; i++)
		drawPoint(x, i, current_color);
}

void
draw_text(int id)
{
	int i, line_len, height_len;
	i = 0;
	line_len = word[id].x_start;
	height_len = word[id].y_start;
	if(word[id].cursor == 0)
	{
		draw_cursor(line_len, height_len);
		word[id].cursor_x = line_len;
		word[id].cursor_y = height_len;
	}
	while(i < WORD_LEN && word[id].text[i] != '\0')
	{
		if(line_len > word[id].width+150 || word[id].text[i] == '\n' || word[id].text[i] == '\r')
		{
			height_len += WINDOW_WIDTH_HEIGHT;
			line_len = word[id].x_start;
		}
		if(word[id].text[i] != '\n' && word[id].text[i] != '\r')
		{
			word[id].text_word_width[i] = drawWord(word[id].text[i], line_len, height_len, current_color);
			line_len += word[id].text_word_width[i];
		}
		if(i == word[id].cursor - 1)
		{
			draw_cursor(line_len, height_len);
			word[id].cursor_x = line_len;
			word[id].cursor_y = height_len;
		}
		i++;
	}
	if(height_len > word[i].height)
		word[i].full = 1;
	else
		word[i].full = 0;
}

void
clear_area(uint x, uint y, uint width, uint height)
{
	int i, j;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			canvas[x+i-window_x-CANVAS_WIDTH_BEGIN][y+j-window_y-CANVAS_HEIGHT_BEGIN] = COLOR_WHITE;
			drawPoint(x+i, y+j, canvas[x+i-window_x-CANVAS_WIDTH_BEGIN][y+j-window_y-CANVAS_HEIGHT_BEGIN]);
		}
	}
	renderScreen(x, y, width, height);
}

// window
void 
clear_canvas(uint x, uint y)
{
	int i, j;
	int screen_x = CANVAS_WIDTH_BEGIN + x;
	int screen_y = CANVAS_HEIGHT_BEGIN + y;
	for(i = 0; i < CANVAS_WIDTH; i++)
	{
		for(j = 0; j < CANVAS_HEIGHT; j++)
		{
			canvas[i][j] = COLOR_WHITE;
			drawPoint(screen_x+i, screen_y+j, canvas[i][j]);
		}
	}
	//cprintf("%d, %d\n", x, y);
	//cprintf("%d, %d\n", screen_x, screen_y);
	renderScreen(screen_x, screen_y, CANVAS_WIDTH, CANVAS_HEIGHT);
}

//paint_write
void
draw_kbd_text(char ch, int id, int flag)
{
	if(flag == KBD_LEFT)
	{
		if(word[id].cursor > 0)
			word[id].cursor--;
	}
	else if(flag == KBD_RIGHT)
	{
		if(word[id].cursor < word[id].text_length)
			word[id].cursor++;
	}
	else if (flag == KBD_UP)
	{
		int i = word[id].cursor - 1;
		int len = 0;
		while(1)
		{
			len += word[id].text_word_width[i];
			if(len > word[id].width || word[id].text[i] == '\0')
			{
				word[id].cursor = i + 1;
				break;
			}
			else if(word[id].text[i] == '\n' || word[id].text[i] == '\r')
			{
				word[id].cursor = i;
				break;
			}
			i--;
		}
	}
	else if (flag == KBD_DOWN)
	{
		int i = word[id].cursor + 1;
		int len = 0;
		while(1)
		{
			len += word[id].text_word_width[i];
			if(len > word[id].width || i == word[id].text_length || word[id].text[i] == '\n' || word[id].text[i] == '\r')
			{
				word[id].cursor = i;
				break;
			}
			i++;
		}		
	}
	else if(flag == KBD_END)
	{
		word[id].cursor = word[id].text_length;
	}
	else if(flag < 0)
		return;
	else if((ch >= 0x20 && ch <= 0x7E) || ch == '\n' || ch == '\r')
		add_char(ch, id);
	else if(ch == '\b')
		delete_char(id);
	clear_area(word[id].x_start, word[id].y_start, word[id].width, word[id].height);
	draw_text(id);
	renderScreen(word[id].x_start, word[id].y_start, word[id].width, word[id].height);
}

int
is_canvas(uint x, uint y)
{
	if(x > CANVAS_WIDTH_BEGIN && x < CANVAS_WIDTH_BEGIN + CANVAS_WIDTH && y > CANVAS_HEIGHT_BEGIN && y < CANVAS_HEIGHT_BEGIN + CANVAS_HEIGHT)
		return 1;
	else
		return 0;
}

int
is_in(uint x, uint y)
{
	if(x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT)	return 1;
	else	return 0;
}

void
set_brush_color(uint x, uint y)
{
	if(x > COLOR_BEGIN_X && x < COLOR_BEGIN_X+10*COLOR_WIDTH+9*COLOR_INTERVAL && y > COLOR_BEGIN_Y && y < COLOR_BEGIN_Y+2*COLOR_HEIGHT)
	{
		if(x > COLOR_BEGIN_X && x < COLOR_BEGIN_X+COLOR_WIDTH)
		{
			if(y > COLOR_BEGIN_Y && y < COLOR_BEGIN_Y+COLOR_HEIGHT)
				current_color = COLOR_BLACK;
			else if(y > COLOR_BEGIN_Y+COLOR_HEIGHT && y < COLOR_BEGIN_Y+2*COLOR_HEIGHT)
				current_color = COLOR_WHITE;
		}
		else if(x > COLOR_BEGIN_X+COLOR_WIDTH+COLOR_INTERVAL && x < COLOR_BEGIN_X+2*COLOR_WIDTH+COLOR_INTERVAL)
		{
			if(y > COLOR_BEGIN_Y && y < COLOR_BEGIN_Y+COLOR_HEIGHT)
				current_color = COLOR_GRAY;
			else if(y > COLOR_BEGIN_Y+COLOR_HEIGHT && y < COLOR_BEGIN_Y+2*COLOR_HEIGHT)
				current_color = COLOR_LIGHTGRAY;

		}
		else if(x > COLOR_BEGIN_X+2*COLOR_WIDTH+2*COLOR_INTERVAL && x < COLOR_BEGIN_X+3*COLOR_WIDTH+2*COLOR_INTERVAL)
		{
			//cprintf("color darkred.\n");
			if(y > COLOR_BEGIN_Y && y < COLOR_BEGIN_Y+COLOR_HEIGHT)
				current_color = COLOR_DARKRED;
			else if(y > COLOR_BEGIN_Y+COLOR_HEIGHT && y < COLOR_BEGIN_Y+2*COLOR_HEIGHT)
				current_color = COLOR_BROWN;

		}
		else if(x > COLOR_BEGIN_X+3*COLOR_WIDTH+3*COLOR_INTERVAL && x < COLOR_BEGIN_X+4*COLOR_WIDTH+3*COLOR_INTERVAL)
		{
			if(y > COLOR_BEGIN_Y && y < COLOR_BEGIN_Y+COLOR_HEIGHT)
				current_color = COLOR_RED;
			else if(y > COLOR_BEGIN_Y+COLOR_HEIGHT && y < COLOR_BEGIN_Y+2*COLOR_HEIGHT)
				current_color = COLOR_PINK;

		}
		else if(x > COLOR_BEGIN_X+4*COLOR_WIDTH+4*COLOR_INTERVAL && x < COLOR_BEGIN_X+5*COLOR_WIDTH+4*COLOR_INTERVAL)
		{
			if(y > COLOR_BEGIN_Y && y < COLOR_BEGIN_Y+COLOR_HEIGHT)
				current_color = COLOR_ORANGE;
			else if(y > COLOR_BEGIN_Y+COLOR_HEIGHT && y < COLOR_BEGIN_Y+2*COLOR_HEIGHT)
				current_color = COLOR_GOLD;

		}
		else if(x > COLOR_BEGIN_X+5*COLOR_WIDTH+5*COLOR_INTERVAL && x < COLOR_BEGIN_X+6*COLOR_WIDTH+5*COLOR_INTERVAL)
		{
			if(y > COLOR_BEGIN_Y && y < COLOR_BEGIN_Y+COLOR_HEIGHT)
				current_color = COLOR_YELLOW;
			else if(y > COLOR_BEGIN_Y+COLOR_HEIGHT && y < COLOR_BEGIN_Y+2*COLOR_HEIGHT)
				current_color = COLOR_LIGHTYELLOW;

		}
		else if(x > COLOR_BEGIN_X+6*COLOR_WIDTH+6*COLOR_INTERVAL && x < COLOR_BEGIN_X+7*COLOR_WIDTH+6*COLOR_INTERVAL)
		{
			if(y > COLOR_BEGIN_Y && y < COLOR_BEGIN_Y+COLOR_HEIGHT)
				current_color = COLOR_GREEN;
			else if(y > COLOR_BEGIN_Y+COLOR_HEIGHT && y < COLOR_BEGIN_Y+2*COLOR_HEIGHT)
				current_color = COLOR_LIGHTGREEN;

		}
		else if(x > COLOR_BEGIN_X+7*COLOR_WIDTH+7*COLOR_INTERVAL && x < COLOR_BEGIN_X+8*COLOR_WIDTH+7*COLOR_INTERVAL)
		{
			if(y > COLOR_BEGIN_Y && y < COLOR_BEGIN_Y+COLOR_HEIGHT)
				current_color = COLOR_BLUEGREEN;
			else if(y > COLOR_BEGIN_Y+COLOR_HEIGHT && y < COLOR_BEGIN_Y+2*COLOR_HEIGHT)
				current_color = COLOR_LIGHTBLUEGREEN;

		}
		else if(x > COLOR_BEGIN_X+8*COLOR_WIDTH+8*COLOR_INTERVAL && x < COLOR_BEGIN_X+9*COLOR_WIDTH+8*COLOR_INTERVAL)
		{
			if(y > COLOR_BEGIN_Y && y < COLOR_BEGIN_Y+COLOR_HEIGHT)
				current_color = COLOR_BLUE;
			else if(y > COLOR_BEGIN_Y+COLOR_HEIGHT && y < COLOR_BEGIN_Y+2*COLOR_HEIGHT)
				current_color = COLOR_LIGHTBLUE;

		}
		else if(x > COLOR_BEGIN_X+9*COLOR_WIDTH+9*COLOR_INTERVAL && x < COLOR_BEGIN_X+10*COLOR_WIDTH+9*COLOR_INTERVAL)
		{
			if(y > COLOR_BEGIN_Y && y < COLOR_BEGIN_Y+COLOR_HEIGHT)
				current_color = COLOR_PURPLE;
			else if(y > COLOR_BEGIN_Y+COLOR_HEIGHT && y < COLOR_BEGIN_Y+2*COLOR_HEIGHT)
				current_color = COLOR_LIGHTPURPLE;
		}
		//cprintf("current_color = %x\n", current_color);
	}
}

void
get_command(uint x, uint y)
{
	if(x > 15 && x < 320 && y > 40 && y < 84)
	{
		if(x > 15 && x < 65)
			command = PAINT_PEN;
		else if(x > 75 && x < 125)
			command = PAINT_ERASER;
		else if(x > 145 && x < 195)
		{
			command = PAINT_RECT;
			x_start = y_start = x_end = y_end = 0;
		}
		else if(x > 200 && x < 250)
		{
			command = PAINT_TEXT;
			x_start = y_start = x_end = y_end = 0;
			text_id++;
			if(text_id >= TEXT_LEN)
				command = PAINT_PEN;
		}
		else if(x > 260 && x < 320)
		{
			command = PAINT_RESET;
			clear_canvas(window_x, window_y);
			text_id = 0;
			command = PAINT_PEN;
		}
		//cprintf("command = %d\n", command);
	}
}

void
draw_scale_word(char ch, int flag)
{
	switch(flag)
	{
		case KBD_INC:
			line_width = line_width + 2 > LINE_MAX ? LINE_MAX : (line_width + 2);
			break;
		case KBD_DEC:
			line_width = line_width - 2 < LINE_WIDTH ? LINE_WIDTH : (line_width - 2);
			break;
		case KBD_ESC:
			current_color = COLOR_WHITE;
			draw_cursor(word[text_id].cursor_x, word[text_id].cursor_y);
			renderScreen(word[text_id].x_start, word[text_id].y_start, word[text_id].width, word[text_id].height);
			command = PAINT_PEN;
			break;
		default:
			if(command == PAINT_TEXT || command == PAINT_WRITE)
				draw_kbd_text(ch, text_id, flag);
			break;
	}
	cprintf("command=%d\n", command);
}

void
draw_big_point(uint x, uint y)
{
	int i, j;
	int draw_x = window_x + x;
	int draw_y = window_y + y;
	int add_width = line_width - LINE_WIDTH;
	x = x - CANVAS_WIDTH_BEGIN;
	y = y - CANVAS_HEIGHT_BEGIN;
	for(i = 0; i < LINE_WIDTH; i++)
	{
		for(j = 0; j < LINE_WIDTH; j++)
		{
			if(i < 3 && j < 3)
			{
				if(circle[i][j] != 0 && x+i-line_width >= 0 && y+j-line_width >= 0 && x+i-line_width <= CANVAS_WIDTH && y+j-line_width <= CANVAS_HEIGHT)
				{
					canvas[x+i-line_width][y+j-line_width] = current_color;
					drawPoint(draw_x+i-line_width, draw_y+j-line_width, current_color);
					renderScreen(draw_x+i-line_width, draw_y+j-line_width, 1, 1);
				}
			}
			else if(i < 3 && j > 2)
			{
				if(circle[i][j] != 0 && x+i-line_width >= 0 && y+j-LINE_WIDTH >= 0 && x+i-line_width <= CANVAS_WIDTH && y+j-LINE_WIDTH <= CANVAS_HEIGHT)
				{
					canvas[x+i-line_width][y+j-LINE_WIDTH] = current_color;
					drawPoint(draw_x+i-line_width, draw_y+j-LINE_WIDTH, current_color);
					renderScreen(draw_x+i-line_width, draw_y+j-LINE_WIDTH, 1, 1);
				}
			}
			else if(i > 2 && j < 3)
			{
				if(circle[i][j] != 0 && x+i-LINE_WIDTH >= 0 && y+j-line_width >= 0 && x+i-LINE_WIDTH <= CANVAS_WIDTH && y+j-line_width <= CANVAS_HEIGHT)
				{
					canvas[x+i-LINE_WIDTH][y+j-line_width] = current_color;
					drawPoint(draw_x+i-LINE_WIDTH, draw_y+j-line_width, current_color);
					renderScreen(draw_x+i-LINE_WIDTH, draw_y+j-line_width, 1, 1);
				}
			}
			else if (i > 2 && j > 2)
			{
				if(circle[i][j] != 0 && x+i-LINE_WIDTH >= 0 && y+j-LINE_WIDTH >= 0 && x+i-LINE_WIDTH <= CANVAS_WIDTH && y+j-LINE_WIDTH <= CANVAS_HEIGHT)
				{
					canvas[x+i-LINE_WIDTH][y+j-LINE_WIDTH] = current_color;
					drawPoint(draw_x+i-LINE_WIDTH, draw_y+j-LINE_WIDTH, current_color);
					renderScreen(draw_x+i-LINE_WIDTH, draw_y+j-LINE_WIDTH, 1, 1);
				}
			}
		}
	}
	if(add_width > 0)
	{
		for(i = 0; i < add_width; i++)
		{
			for(j = 0; j < 3; j++)
			{
				if(x+i-line_width+3 >= 0 && y+j-line_width >= 0 && x+i-line_width+3 <= CANVAS_WIDTH && y+j-line_width <= CANVAS_HEIGHT)
				{
					canvas[x+i-line_width+3][y+j-line_width] = current_color;
					drawPoint(draw_x+i-line_width+3, draw_y+j-line_width, current_color);
					renderScreen(draw_x+i-line_width+3, draw_y+j-line_width, 1, 1);								}
			}
			for(j = 1; j < 4; j++)
			{
				if(x+i-line_width+3 >= 0 && y-j >= 0 && x+i-line_width+3 <= CANVAS_WIDTH && y-j <= CANVAS_HEIGHT)
				{
					canvas[x+i-line_width+3][y-j] = current_color;
					drawPoint(draw_x+i-line_width+3, draw_y-j, current_color);
					renderScreen(draw_x+i-line_width+3, draw_y-j, 1, 1);	
				}
			}
		}
		for(i = 0; i < line_width; i++)
		{
			for(j = 0; j < add_width; j++)
			{
				if(x+i-line_width >= 0 && y+j-line_width+3 >= 0 && x+i-line_width <= CANVAS_WIDTH && y+j-line_width+3 <= CANVAS_HEIGHT)
				{
					canvas[x+i-line_width][y+j-line_width+3] = current_color;
					drawPoint(draw_x+i-line_width, draw_y+j-line_width+3, current_color);
					renderScreen(draw_x+i-line_width, draw_y+j-line_width+3, 1, 1);	
				}
			}
		}
	}
}

void
draw(uint x, uint y, uint win_x, uint win_y, struct EventState* mouse)
{
	window_x = win_x;
	window_y = win_y;
	if(is_in(x, y))
	{
		if(!is_canvas(x, y))
		{
			if(mouse->isClick)
			{
				//cprintf("x=%d,y=%d\n", x, y);
				get_command(x, y);
				set_brush_color(x, y);
			}			
		}
		else
		{
			//cprintf("x_start=%d, y_start=%d, x_end=%d, y_end=%d\n", x_start, y_start, x_end, y_end);
			if(command == PAINT_PEN)
			{
				if(current_color == COLOR_WHITE)
					current_color = COLOR_BLACK;
				if(mouse->isClick || mouse->isDragging)
				{
					draw_big_point(x, y);
				}
			}
			else if(command == PAINT_ERASER)
			{
				current_color = COLOR_WHITE;
				if(mouse->isClick || mouse->isDragging)
				{
					draw_big_point(x, y);
				}
			}
			else if(command == PAINT_RECT)
			{
				if(current_color == COLOR_WHITE)
					current_color = COLOR_BLACK;
				if(mouse->isClick == 1 && mouse->isDragging == 0)
				{
					x_start = mouse->x_start;
					y_start = mouse->y_start;
				}
				if(mouse->isClick == 0 && mouse->isDragging == 0)
				{
					x_end = mouse->x_end;
					y_end = mouse->y_end;
					if(max(x_start, x_end) - min(x_start, x_end) < line_width*2 || max(y_start, y_end) - min(y_start, y_end) < line_width*2 || x_start < CANVAS_WIDTH_BEGIN || y_start < CANVAS_HEIGHT_BEGIN || x_end < CANVAS_WIDTH_BEGIN || y_end < CANVAS_HEIGHT_BEGIN)
						return;
					int i;
					for(i = min(x_start, x_end); i <= max(x_start, x_end); i++)
					{
						draw_big_point(i-window_x, y_start-window_y);
						draw_big_point(i-window_x, y_end-window_y);
					}
					for(i = min(y_start, y_end); i <= max(y_start, y_end); i++)
					{
						draw_big_point(x_start-window_x, i-window_y);
						draw_big_point(x_end-window_x, i-window_y);
					}
					command = PAINT_PEN;
				}
			}
			else if(command == PAINT_TEXT)
			{
				if(current_color == COLOR_WHITE)
					current_color = COLOR_BLACK;
				if(mouse->isClick == 1 && mouse->isDragging == 0)
				{
					x_start = mouse->x_start;
					y_start = mouse->y_start;
				}
				if(mouse->isClick == 0 && mouse->isDragging == 0)
				{
					x_end = mouse->x_end;
					y_end = mouse->y_end;
					if(max(x_start, x_end) - min(x_start, x_end) < line_width*2 || max(y_start, y_end) - min(y_start, y_end) < line_width*2 || x_start < CANVAS_WIDTH_BEGIN || y_start < CANVAS_HEIGHT_BEGIN || x_end < CANVAS_WIDTH_BEGIN || y_end < CANVAS_HEIGHT_BEGIN)
						return;
					// int i;
					// for(i = min(x_start, x_end); i <= max(x_start, x_end); i++)
					// {
					// 	draw_big_point(i-window_x, y_start-window_y);
					// 	draw_big_point(i-window_x, y_end-window_y);
					// }
					// for(i = min(y_start, y_end); i <= max(y_start, y_end); i++)
					// {
					// 	draw_big_point(x_start-window_x, i-window_y);
					// 	draw_big_point(x_end-window_x, i-window_y);
					// }
					init(text_id, x_start, x_end, y_start, y_end);
					command = PAINT_WRITE;
				}
			}
		}
	}
}
