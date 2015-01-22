/**************************************************************************
**  window.c
**  multiple window list
**  Author: Wang Chengpeng, Yang Yanzhe
**  Revise Time: 2015.1.21
***************************************************************************/

#include "types.h"
#include "defs.h"
#include "window.h"
#include "node.h"
#include "gui.h"

static int offsetY = 20;
static int offsetX = 20;
static struct Window* active;

void initWindow()
{
	int i;
	for(i = 0; i < MAX_WINDOW; i++)
		window[i].type = -1;
	NumOfWindow = 0;

	WindowLine = requireWindow();
	WindowLine->type = 0;
	
	WindowLine->folder = 0;
	WindowLine->next = 0;
	WindowLine->pre = 0;

	WindowLine->x =0;
	WindowLine->y = 0;
}

struct Window* requireWindow()
{
	if(MAX_WINDOW <= NumOfWindow)
		return 0;

	int i;
	for(i = 0; i < MAX_WINDOW ; i++)
	{
		if(window[i].type == -1)
			break;
	}

	if(i == MAX_WINDOW)  return 0;
	else
	{
		NumOfWindow++;
		return &window[i];
	}
}

struct Window* addWindow(int icon)
{
	if(icon > WINDOW_COMPUTER)
	{
		struct Window* temp = getWindowByType(icon);
		if(temp != 0)
		{
			setFocus(temp);
			return temp;
		}
	}

	struct Window* nw = requireWindow();
	if(!nw)
	{
		nw = getWindowByType(icon);
                active = nw;
		return nw;
	}
	nw->type = icon;

	if(WindowLine->next == 0)
	{
		WindowLine->next = nw;
		nw->next = 0;
		nw->pre = 0;
		nw->folder = 0;
		nw->x = WINDOW_X;
		nw->y = WINDOW_Y;
	}
	else
	{
		struct Window* ptr = WindowLine->next;

		nw->pre = 0;
		nw->next = ptr;
		nw->folder = 0;
		ptr->pre = nw;
		WindowLine->next = nw;

		//保持窗口间的间距，方便窗口选择点击
		nw->x = offsetX + ptr->x;
		nw->y = offsetY + ptr->y;

		if(nw->x + WINDOW_WIDTH > SCREEN_WIDTH){
			nw->x = WINDOW_X;
		}
		if(nw->y + WINDOW_HEIGHT > SCREEN_HEIGHT){
			nw->y = WINDOW_Y;
		}
	}
	active = nw;
	return nw;
}

void releaseWindow(struct Window* w)
{
	if(NumOfWindow==0)	return;
	NumOfWindow--;

	w->type = -1;
	w->folder = 0;
	w->pre = 0;
	w->next = 0;
	w->x = 0;
	w->y = 0;
}

void closeWindow()
{
	if(WindowLine->next == 0) 
		return;

	struct Window* ptr = WindowLine->next;
	struct Window* p = ptr->next;

	if(p == 0)
	{
		releaseWindow(ptr);
		WindowLine->next = 0;
	}
	else
	{
		releaseWindow(ptr);
		WindowLine->next = p;
		p->pre = 0;
	}
}

void setWindowPosition(struct Window* aim, int offX, int offY)
{
	if (!aim)
	{
		aim->x += offX;
		aim->y += offY;
	}
}

void setWindowNode(struct Window* w, struct Node* node)
{
	if (w == 0)			
		return;
	if (node->NodeType == 1)	
		return;

	if (node->NodeType == 0)
		w->folder = node;
}

void settype(struct Window* w, int type)
{
	if (w == 0)
		return;
	if (type < 0)
		return;

	w->type = type;
}

void setFocus(struct Window* aim)
{
	if(WindowLine->next == aim) 
		return;
	active = aim;

	struct Window* ptr = WindowLine->next;
	struct Window* pre = aim->pre;
	pre->next = aim->next;
	if(aim->next != 0)
		aim->next->pre = pre;

	WindowLine->next = aim;
	aim->pre = 0;
	aim->next = ptr;
	ptr->pre = aim;
}

struct Window* getLastWindow()
{
	struct Window* p;
	struct Window* q;
	for (p = WindowLine->next, q = p; p != 0; p = p->next)
	{
		q = p;
	}
	return q;
}

struct Window* getWindowByPosition(int px, int py)
{
	int x, y;
	struct Window* ptr = WindowLine->next;

	while(ptr)
	{
		x = ptr->x;
		y = ptr->y;
		if (px <= x + WINDOW_WIDTH && px >= x && py <= y + WINDOW_HEIGHT && py >= y)
			break;
		ptr = ptr->next;
	}
	return ptr;
}

struct Window* getWindowByType(int type)
{
	struct Window* ptr = WindowLine->next;
	
	for (ptr = WindowLine->next; ptr != 0; ptr = ptr->next)
	{
		if (ptr->type == type)
			break;
	}
	return ptr;
}

struct Window* getActiveWindow()
{
	return active;
}

