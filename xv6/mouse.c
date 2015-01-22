/**************************************************************************
**  mouse.c
**  PS/2 mouse driver
**  author: yangyanzhe
**  time:   2014.12.28
**  reference: [1]www.eecg.toronto.edu/~jayar/ece241_08F/AudioVideoCores/ps2/ps2.html
**             [2]http://www.oldlinux.org/oldlinux/archiver/?tid-13138.html
**             [3]houbysoft.com/download/ps2mouse.html
***************************************************************************/

#include "types.h"
#include "defs.h"
#include "x86.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "mouse.h"
#include "gui.h"

//#define DEBUG 0

static struct spinlock mouselock;
static struct MousePosition mouse_pos;
static struct MousePosition mouse_pos_final;

static struct Mouse packet;
static struct EventState history;
//static uchar data_union[3];
static uint count; 
static uint currentApp;
enum MouseState mouse_state;

int MOUSE_POSX_MAX;
int MOUSE_POSY_MAX;

void mouse_reset(void);

void
mouse_wait(uchar type)
{
    uint time_out = 100000;
    if(type == 0)
    {
        while(--time_out)
        {
            if((inb(0x64) & 1) == 1)
                return;
        }
    }
    else
    {
        while(--time_out)
        {
            if((inb(0x64) & 2) == 0)
                return;
        }   
    }
}

void
mouse_write(uchar word)
{
    mouse_wait(1);
    outb(0x64, 0xd4);
    mouse_wait(1);
    outb(0x60, word);
}

uint
mouse_read()
{
    mouse_wait(0);
    return inb(0x60);
}


void
mouse_refresh(void)
{
    if((mouse_state == IGNORE ))
    {
        //release(&mouselock);
        return;
    }

    mouse_pos_final.x = mouse_pos.x;
    mouse_pos_final.y = mouse_pos.y;

    drawMouse(mouse_pos.x, mouse_pos.y);
}


void
mouse_pos_init(void)
{
    // set the initialized value
    mouse_pos.x = SCREEN_WIDTH / 2;
    mouse_pos.y = SCREEN_HEIGHT / 2;

    MOUSE_POSX_MAX = SCREEN_WIDTH - CURSOR_WIDTH;
    MOUSE_POSY_MAX = SCREEN_HEIGHT - CURSOR_HEIGHT;
}

void
mouse_handler(void)
{
    int x, y;

    x = packet.x_sign ? (0xffffff00 | (packet.x_movement & 0xff)) : (packet.x_movement & 0xff);
    y = packet.y_sign ? (0xffffff00 | (packet.y_movement & 0xff)) : (packet.y_movement & 0xff);

    if(x == 127 || x == -127 || y == 127 || y == -127){
        x = 0;
        y = 0;
    }
    
    mouse_pos.x += x;
    mouse_pos.y -= y;

    if(mouse_pos.x <= 0)    mouse_pos.x = 0;
    else if(mouse_pos.x >= MOUSE_POSX_MAX)    mouse_pos.x = MOUSE_POSX_MAX;
 
    if(mouse_pos.y <= 0)    mouse_pos.y = 0;
    else if(mouse_pos.y >= MOUSE_POSY_MAX)    mouse_pos.y = MOUSE_POSY_MAX;

    if(packet.btn_left)
    {
        history.x_start = mouse_pos.x;
        history.y_start = mouse_pos.y;
        history.btn_left_down = 1;
        event_left_btn_down();
    }
    else if(history.btn_left_down)
    {
        history.x_end = mouse_pos.x;
        history.y_end = mouse_pos.y;
        history.btn_left_down = 0;
	    event_left_btn_up();
    }    

    if(packet.btn_right)
    {
        event_right_btn_down();
        history.btn_right_down = 1;
    }
    else if( history.btn_right_down)
    {
         history.btn_right_down = 0;
         event_right_btn_up();
    }
    
<<<<<<< HEAD
    switch(currentApp)
    {
        case 4:   draw(mouse_pos.x - WINDOW_X, mouse_pos.y - WINDOW_Y, WINDOW_X, WINDOW_Y, &history);
		  //cprintf("draw :%d, %d\n", mouse_pos.x, mouse_pos.y);
         	  break;
        default:  break;
    }
=======
>>>>>>> origin/master

#ifdef DEBUG
    cprintf("mouse pos: %d, %d\n", mouse_pos.x, mouse_pos.y);
#endif
    //mouse_refresh();
    mouse_pos_final.x = mouse_pos.x;
    mouse_pos_final.y = mouse_pos.y;

    drawMouse(mouse_pos.x, mouse_pos.y);
}

void
mouseinit(void)
{
    mouse_pos_init();
    uchar statustemp;

    mouse_wait(1);
    outb(0x64, 0xa8);		//激活鼠标接口
    
    mouse_wait(1);		//激活中断
    outb(0x64, 0x20);
    mouse_wait(0);
    statustemp = (inb(0x60) | 2);
    mouse_wait(0);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, statustemp);

    mouse_write(0xf6);		//设置鼠标为默认设置
    mouse_read();

    mouse_write(0xf3);		//设置鼠标采样率
    mouse_read();
    mouse_write(10);
    mouse_read();
    
    //outb(0x64, 0xd4);		//下一个数据发给鼠标
    //outb(0x60, 0xf4);		//允许鼠标发送数据
    mouse_write(0xf4);
    mouse_read();    
    //inb(0x60);
    //outb(0x64, 0x60); 	//下一个数据写入鼠标控制寄存器
    //outb(0x60, 0x47);		//允许键盘和鼠标，允许中断

    initlock(&mouselock, "mouse");
#ifdef DEBUG
    cprintf("init mouse lock.\n");
#endif
    picenable(IRQ_MOUS);
    ioapicenable(IRQ_MOUS, 0);

    mouse_handler();		// 设置鼠标句柄
    mouse_state = ACTIVE;
    currentApp = 0;
}

void
mouseintr(void)
{
    if(mouse_state == IGNORE)    return;

    acquire(&mouselock);
//#ifdef DEBUG
//    cprintf("acquire mouse lock in mouseintr.\n");
//#endif
    uint data = inb(0x60);
    count++;

    switch(count)
    {
        case 1: if(data & 0x08)
                {
                    //data_union[0] = data; 
                    packet.y_overflow = data >> 7 & 0x1;
                    packet.x_overflow = data >> 6 & 0x1;
                    packet.y_sign = data >> 5 & 0x1;
                    packet.x_sign = data >> 4 & 0x1;
                    packet.btn_mid = data >> 2 & 0x1;
                    packet.btn_right = data >> 1 & 0x1;
                    packet.btn_left = data >> 0 & 0x1;
                    break;
                }
                else
                {
                    count = 0;
#ifdef DEBUG
                    cprintf("bad packet found in mouseintr.\n");
                    cprintf("|%d|%d|%d|%d|%d|%d|%d|%d|\n",
                             data >> 7 & 0x1, data >> 6 & 0x1,
                             data >> 5 & 0x1, data >> 4 & 0x1,
                             data >> 3 & 0x1, data >> 2 & 0x1,
                             data >> 1 & 0x1, data >> 0 & 0x1);
#endif
                    break;
                 }
                 
        case 2:  packet.x_movement = data;
                 break;
        case 3:  packet.y_movement = data;  
                 count = 0;
#ifdef DEBUG
                 mouse_print(&packet);
#endif
                 mouse_handler();
                 break;
        default: count=0;    break;
    }

    release(&mouselock);
//#ifdef DEBUG
//    cprintf("release mouse lock in mouseintr.\n");
//#endif
}

void
mousereset(void)
{
    mouse_state = IGNORE;
    
    // disable it
    mouse_write(0xf5);
    mouse_read();

    mouse_write(0xf6);
    mouse_read();

    mouse_write(0xf4);
    mouse_read();

    mouse_state = ACTIVE;
}

void
mouse_print(struct Mouse* p)
{
    //%x means number displayed in hex
    cprintf("*******Mouse Package Received ******* \n\
            Byte 1: |%d|%d|%d|%d|%d|%d|%d|%d|\n\
            --------------------------------------\n\
            Byte 2: %x \n\
            Byte 3: %x \n",
            p->y_overflow, p->x_overflow, p->y_sign, p->x_sign,
            1, p->btn_mid, p->btn_right, p->btn_left, 
            p->x_movement, p->y_movement);
}

void
event_left_btn_up(void)
{
    uint x = mouse_pos.x;
    uint y = mouse_pos.y;
    history.isDragging = 0;
    history.isClick = 0;

    if(x <= ICON_X1 + ICON_WIDTH && x >= ICON_X1)
    {
        if(y >= ICON_Y5 && y <= ICON_Y5 + ICON_HEIGHT)
        {
#ifdef DEBUG
            cprintf("ICON 5 clicked.\n");
#endif
        }
        else if(y >= ICON_Y4 && y <= ICON_Y4 + ICON_HEIGHT)
        {
		currentApp = 4;
		drawWindow(2, WINDOW_X, WINDOW_Y);
		renderScreen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);	
		
        }
        else if(y >= ICON_Y3 && y <= ICON_Y3 + ICON_HEIGHT)
        {
		currentApp = 3;
		drawWindow(1, WINDOW_X, WINDOW_Y);
		// drawWord('a', WINDOW_X+25, WINDOW_Y+40, 0);
		// drawWord('b', WINDOW_X+WORD_GAP+25, WINDOW_Y+40, 0);
		// drawWord('A', WINDOW_X+2*WORD_GAP+25, WINDOW_Y+40, 0);
		// drawWord('B', WINDOW_X+3*WORD_GAP+25, WINDOW_Y+40, 0);
		initText(WINDOW_X, WINDOW_Y);
        renderScreen(WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
        else if(y >= ICON_Y2 && y <= ICON_Y2 + ICON_HEIGHT)
        {
#ifdef DEBUG
            cprintf("ICON 2 clicked.\n");
#endif
        }
        else if(y >= ICON_Y1 && y <= ICON_Y1 + ICON_HEIGHT)
        {
#ifdef DEBUG
            cprintf("ICON 1 clicked.\n");
#endif
        }
    }
}

void event_left_btn_down(void)
{
    history.isClick = 1;

    if(packet.x_movement!=0 || packet.y_movement != 0){
    history.isDragging = 1;
#ifdef DEBUG
        cprintf("Left is dragging.\n");
#endif
    }
}

void event_right_btn_down(void)
{
     if(packet.x_movement!=0 || packet.y_movement != 0){
    	history.isDragging = 1;
#ifdef DEBUG
        cprintf("Right is dragging.\n");
#endif
    }
}

void event_right_btn_up(void)
{
    history.isDragging = 0;
#ifdef DEBUG
    cprintf("Right button up.\n");
#endif
}
