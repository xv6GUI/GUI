/**************************************************************************
**  mouse.h
**  PS/2 mouse driver
**  author: yangyanzhe
**  time:   2014.12.28
**  reference: [1]www.eecg.toronto.edu/~jayar/ece241_08F/AudioVideoCores/ps2/ps2.html
**             [2]http://www.oldlinux.org/oldlinux/archiver/?tid-13138.html
**             [3]houbysoft.com/download/ps2mouse.html
***************************************************************************/

#ifndef MOUSE_H
#define MOUSE_H

struct Mouse{
  uint btn_left;     // bit 0, if 1 then left button down
  uint btn_right;    // bit 1, if 1 then right button down
  uint btn_mid;	     // bit 2, if 1 then mid button down
  uint always1 : 1;
  uint x_sign;       // bit 4, if 1 then mouse move toward left 
  uint y_sign;       // bit 5, if 1 then mouse move toward top
  uint x_overflow;   // bit 6, if 1 then x out of range
  uint y_overflow;   // bit 7, if 1 then y out of range
  uint x_movement;
  uint y_movement;
};

struct EventState{
  uint isDragging;
  uint isClick;
  uint btn_left_down;  	   // if 1 then left button down
  uint btn_right_down;     // if 1 then right button down

  uint x_start;	           // if left button down, then record start
  uint y_start;
  uint x_end;              // if left button up, then record end
  uint y_end;  
};

struct MousePosition{
  int x;
  int y;
};

enum MouseState{
    IGNORE = 0,
    ACTIVE,
};

void mouse_print(struct Mouse* p);
void event_left_btn_down(void);
void event_right_btn_down(void);
void event_right_btn_up(void);
void event_left_btn_up(void);
int getAppNum(struct Window* cur);

#endif

