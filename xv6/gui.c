#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
#include "gui.h"
#include "CHARACTER.h"
#include "ICON.h"
#include "BACKGROUND.h"
#include "window.h"
#include "PPT.h"

static unsigned short *VESA_ADDR = (unsigned short*)0xe0000000;
unsigned short SCREEN_COLOR[SCREEN_WIDTH][SCREEN_HEIGHT];

//original mouse position
int ori_x_mouse = SCREEN_WIDTH / 2, ori_y_mouse = SCREEN_HEIGHT / 2;

void 
display_to_screen(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
  int i, j;
  for(i = 0; i < width; ++i)
  {
    for(j = 0; j < height; ++j)
    {
      unsigned int offset = (j + y) * SCREEN_WIDTH + (i + x);
      *(VESA_ADDR + offset) = SCREEN_COLOR[i + x][j + y];
    }
  }
}

void 
draw_point(unsigned int x, unsigned int y, unsigned short color)
{
  if(x >= SCREEN_WIDTH) 
    return;
  if(y >= SCREEN_HEIGHT)
    return;
  SCREEN_COLOR[x][y] = color;
}

void
draw_background_point(unsigned int x, unsigned int y, int id)
{
  if(y < 75)
    draw_point(x, y, 0);
  else if(y >= SCREEN_HEIGHT - 75)
    draw_point(x, y, 0);
  else 
  {
    draw_point(x, y, PPT[id][x][y - 75]);
  }
}

int
draw_character(unsigned int x, unsigned int y, char ch, unsigned short color)
{
  if(x + CHARACTER_WIDTH > SCREEN_WIDTH)
    return -1;
  if(y + CHARACTER_HEIGHT > SCREEN_HEIGHT)
    return -1;
  int id = ch - 0x20;
  if(id > 94 || id < 0)
    return -1;

  int i, j;
  int tmp = 0;
  int draw_flag = 1;
  i = 0;
  while(draw_flag == 1 && i < CHARACTER_WIDTH)
  {
    //ignore some rows of data according to the character's width
    if(i >= 4)
    {
      tmp = 0;
      for(j = 0; j < CHARACTER_HEIGHT; j++)
      {
        if(character[id][j][i] != 0)
        {
          tmp = 1;
          break;
        }
      }
      if(tmp == 1)
        draw_flag = 1;
      else
        draw_flag = 0;
    }

    if(draw_flag == 1)
    {
      for(j = 0; j < CHARACTER_HEIGHT; j++)
      {
        if(character[id][j][i] == 0)
          continue;
        else
        {
          draw_point(x + i, y + j, color);
        }
      }
    }
    i++;
  }
  return (i);
}

void
draw_string(unsigned int x, unsigned int y, char *str, unsigned short color)
{
  int i;
  int offset_x = 0, offset_y = 0;
  for(i = 0; i < strlen(str); ++i)
  {
    if(x + offset_x + CHARACTER_WIDTH > SCREEN_WIDTH)
    {
      offset_x = 0;
      offset_y += CHARACTER_HEIGHT;
    }
    if(y + offset_y + CHARACTER_HEIGHT > SCREEN_HEIGHT)
      return;
    offset_x += draw_character(x + offset_x, y + offset_y, str[i], color);
  }
}

void
draw_window(struct Window* window)
{
  unsigned int x = window->Pos_x;
  unsigned int y = window->Pos_y;
  int icon_no = window->Cur_icon;
  int i, j;
  for(i = 0; i < WindowWidth; i++)
  {
    draw_point(i + x, y, 0);
    draw_point(i + x, WindowHeight - 1 + y, 0);
    for(j = 1; j < 20; j++)
      draw_point(i + x, j + y, 0x5ACB);
    for(j = 2; j <= 20; j++)
      draw_point(i + x, y + WindowHeight - j, 0x5ACB);
  }
  for(j = 0; j < WindowHeight; j++)
  {
    draw_point(x, j + y, 0);
    draw_point(WindowWidth - 1 + x, j + y, 0);
  }
  for(i = 3; i < 18; i++)
  {
    for(j = 3; j < 18; j++)
    {
      if(window_button_x[i-3][j-3] != 2016)
        draw_point(i + x, j + y, window_button_x[i-3][j-3]);
    }
  }

  int offset = 25;
  switch(icon_no)
  {
    case ICON_FINDER:
      draw_string(x + offset, y + 2, "FINDER", FONT_COLOR);
      InitFolder(window);
      break;
    case ICON_PHOTO:
      draw_string(x + offset, y + 2, "PHOTO", FONT_COLOR);
      init_photo(x, y);
      break;
    case ICON_TEXT:
      draw_string(x + offset, y + 2, "TEXT", FONT_COLOR);
      init_text(x, y);
      break;
    case ICON_GAME:
      draw_string(x + offset, y + 2, "GAME", FONT_COLOR);
      initGameWindow(x, y);
      break;
    case ICON_DRAW:
      draw_string(x + offset, y + 2, "DRAW", FONT_COLOR);
      init_draw(x, y);
      break;
    case ICON_SETTING:
      draw_string(x + offset, y + 2, "SETTING", FONT_COLOR);
      init_setting(x, y);
      break;
    case 8:
      draw_string(x + offset, y + 2, "FINDER", FONT_COLOR);
      InitFolder(window);
      break;
    case 9:
      draw_string(x + offset, y + 2, "FINDER", FONT_COLOR);
      InitFolder(window);
      break;
  }

  draw_string(x + 590, y + 380, "@meizhi", 0xFFFF);
}

void
clean_mouse()
{
  int i, j;
  for(i = ori_x_mouse; i < ori_x_mouse + SIZE_X_MOUSE;++i)
  {
    for(j = ori_y_mouse; j < ori_y_mouse + SIZE_Y_MOUSE;++j)
    {
      if(mouse[i - ori_x_mouse][j - ori_y_mouse] != 2016)
        display_to_screen(i, j, 1, 1);
    }
  }
}

int 
is_in_canvas(struct Window* window, unsigned int x, unsigned int y)
{
  x -= window->Pos_x;
  y -= window->Pos_y;
  if(x >= 30 && x <= 20 + CANVAS_WIDTH
    && y >= 70 && y <= 60 + CANVAS_HEIGHT)
    return 1;
  else
    return 0;
}

void 
draw_mouse(int pos_x, int pos_y)
{
  clean_mouse();

  struct Window* focused = WindowLine->next;
  if(focused != 0)
  {
    if(focused->Cur_icon == ICON_DRAW)
    {
      if(is_in_canvas(focused, ori_x_mouse, ori_y_mouse))        
        display_to_screen(ori_x_mouse, ori_y_mouse, SIZE_X_MOUSE, SIZE_Y_MOUSE);
      if(is_in_canvas(focused, pos_x, pos_y))
      {
        draw_brush(pos_x, pos_y);
        ori_x_mouse = pos_x;
        ori_y_mouse = pos_y;
        return; 
      }  
    }
  }

  int i, j;
  for(i = 0; i < SIZE_X_MOUSE; ++i)
  {
    for(j = 0; j < SIZE_Y_MOUSE; ++j)
    {
      if(mouse[i][j] != 2016)
      {
        unsigned int offset = (j + pos_y) * SCREEN_WIDTH + (i + pos_x);
        *(VESA_ADDR + offset) = mouse[i][j];
      }
    }
  }
  ori_x_mouse = pos_x;
  ori_y_mouse = pos_y;
}

void
draw_icon(int flag, int pos_x, int pos_y)
{
  int i;
  int j;
  for(i = 0; i < ICON_WIDTH; ++i)
  {
    for(j = 0; j < ICON_WIDTH; ++j)
    {
      /*int tmp_g = (icon[flag][i][j] & 2016) >> 5;
      int tmp_r = icon[flag][i][j] >> 11;
      int tmp_b = icon[flag][i][j] & 31;*/
      if(icon[flag][i][j] != 2016)//!(tmp_g >= 55 && tmp_r <= 15 && tmp_b <= 15))
      {
        //cprintf("%d %d %d\n", tmp_r, tmp_g, tmp_b);
        draw_point(pos_x + i, pos_y + j, icon[flag][i][j]);
      }
      else
      {
        continue;
      }
    }
  }
}

void 
draw_background(int id)
{
  int i;
  int j;
  for(i = 0; i < SCREEN_WIDTH; ++i)
  {
    for(j = 0; j < SCREEN_HEIGHT; ++j)
    {
      draw_point(i, j, BACKGROUND[id][i][j]);
    }
  }
}

void
draw_bottom(int id)
{
  /*int i, j;
  for(i = 0; i < SCREEN_WIDTH; ++i)
  {
    for(j = 0; j < SCREEN_HEIGHT; ++j)
    {
      unsigned int offset = j * SCREEN_WIDTH + i;
      *(VESA_ADDR + offset) = RECORD_COLOR[i][j];
    }
  }*/
  draw_background(id);

  unsigned short color = FONT_COLOR;
  if(id >= 3)
    color = 0x0000;
  draw_icon(ICON_FINDER, ICON_X1, ICON_Y);
  draw_string(ICON_X1 + 12, ICON_STRING_Y, "FINDER", color);
  draw_icon(ICON_PHOTO, ICON_X2, ICON_Y);
  draw_string(ICON_X2 + 12, ICON_STRING_Y, "PHOTO", color);
  draw_icon(ICON_TEXT, ICON_X3, ICON_Y);
  draw_string(ICON_X3 + 20, ICON_STRING_Y, "TEXT", color);
  draw_icon(ICON_GAME, ICON_X4, ICON_Y);
  draw_string(ICON_X4 + 17, ICON_STRING_Y, "GAME", color);
  draw_icon(ICON_DRAW, ICON_X5, ICON_Y);
  draw_string(ICON_X5 + 17, ICON_STRING_Y, "DRAW", color);
  draw_icon(ICON_SETTING, ICON_X6, ICON_Y);
  draw_string(ICON_X6 + 8, ICON_STRING_Y, "SETTING", color);

  draw_mouse(ori_x_mouse, ori_y_mouse);

  struct Window* win = Get_LastWindow();
  while(win != 0)
  {
    if(win->pre != 0)
      draw_window(win);
    win = win->pre;
  }
  
  //display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void
draw_again(int id)
{
  draw_background(id);

  unsigned short color = FONT_COLOR;
  if(id >= 3)
    color = 0x0000;
  draw_icon(ICON_FINDER, ICON_X1, ICON_Y);
  draw_string(ICON_X1 + 12, ICON_STRING_Y, "FINDER", color);
  draw_icon(ICON_PHOTO, ICON_X2, ICON_Y);
  draw_string(ICON_X2 + 12, ICON_STRING_Y, "PHOTO", color);
  draw_icon(ICON_TEXT, ICON_X3, ICON_Y);
  draw_string(ICON_X3 + 20, ICON_STRING_Y, "TEXT", color);
  draw_icon(ICON_GAME, ICON_X4, ICON_Y);
  draw_string(ICON_X4 + 17, ICON_STRING_Y, "GAME", color);
  draw_icon(ICON_DRAW, ICON_X5, ICON_Y);
  draw_string(ICON_X5 + 17, ICON_STRING_Y, "DRAW", color);
  draw_icon(ICON_SETTING, ICON_X6, ICON_Y);
  draw_string(ICON_X6 + 8, ICON_STRING_Y, "SETTING", color);

  draw_mouse(ori_x_mouse, ori_y_mouse);

  struct Window* win = Get_LastWindow();
  while(win != 0)
  {
    draw_window(win);
    win = win->pre;
  }
  
  display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void 
draw_loading()
{
  draw_background(0);

  draw_icon(ICON_FINDER, ICON_X1, ICON_Y);
  draw_string(ICON_X1 + 12, ICON_STRING_Y, "FINDER", FONT_COLOR);
  draw_icon(ICON_PHOTO, ICON_X2, ICON_Y);
  draw_string(ICON_X2 + 12, ICON_STRING_Y, "PHOTO", FONT_COLOR);
  draw_icon(ICON_TEXT, ICON_X3, ICON_Y);
  draw_string(ICON_X3 + 20, ICON_STRING_Y, "TEXT", FONT_COLOR);
  draw_icon(ICON_GAME, ICON_X4, ICON_Y);
  draw_string(ICON_X4 + 17, ICON_STRING_Y, "GAME", FONT_COLOR);
  draw_icon(ICON_DRAW, ICON_X5, ICON_Y);
  draw_string(ICON_X5 + 17, ICON_STRING_Y, "DRAW", FONT_COLOR);
  draw_icon(ICON_SETTING, ICON_X6, ICON_Y);
  draw_string(ICON_X6 + 8, ICON_STRING_Y, "SETTING", FONT_COLOR);

  display_to_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  draw_mouse(ori_x_mouse, ori_y_mouse);

  InitNode();
  InitWindow();
}

