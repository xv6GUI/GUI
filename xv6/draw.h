#ifndef DRAW_H
#define DRAW_H

#define COLOR_BLACK	0x0000
#define COLOR_GRAY 0x8410
#define COLOR_DARKRED 0x8002
#define COLOR_RED 0xE8C4
#define COLOR_ORANGE 0xFC05
#define COLOR_YELLOW 0xFF80
#define COLOR_GREEN 0x2589
#define COLOR_BLUEGREEN 0x051D
#define COLOR_BLUE 0x3A39
#define COLOR_PURPLE 0xA254
#define COLOR_WHITE 0xFFFF
#define COLOR_LIGHTGRAY 0xC638
#define COLOR_BROWN 0xBBCA
#define COLOR_PINK 0xFD79
#define COLOR_GOLD 0xFE41
#define COLOR_LIGHTYELLOW 0xF736
#define COLOR_LIGHTGREEN 0xB723
#define COLOR_LIGHTBLUEGREEN 0x9EDD
#define COLOR_LIGHTBLUE 0x7497
#define COLOR_LIGHTPURPLE 0xC5FC

#define black 0
#define gray 1
#define darkRed 2
#define red 3
#define orange 4
#define yellow 5
#define green 6
#define blueGreen 7
#define blue 8
#define purple 9
#define white 10
#define lightGray 11
#define brown 12
#define pink 13
#define gold 14
#define lightYellow 15
#define lightGreen 16
#define lightBlueGreen 17
#define lightBlue 18
#define lightPurple 19

#define LINE_WIDTH 6
#define LINE_MAX 20
#define CANVAS_WIDTH_BEGIN 14
#define CANVAS_HEIGHT_BEGIN 90

#define COLOR_INTERVAL 6
#define COLOR_WIDTH 16
#define COLOR_HEIGHT 20
#define COLOR_BEGIN_X 368
#define COLOR_BEGIN_Y 40

#define WORD_LEN 100
#define TEXT_LEN 10
#define WINDOW_WIDTH_HEIGHT 30		// height of each WINDOW_WIDTH


struct text_word
{
	char text[WORD_LEN];
	int text_word_width[WORD_LEN];
	uint x_start, y_start;
	uint width, height;
	int cursor;
	int text_length;
	uint cursor_x, cursor_y;
	int full;		// ok, 0; width or height beyond, 1;
};

#endif