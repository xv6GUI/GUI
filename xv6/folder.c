#include "types.h"
#include "defs.h"
#include "window.h"
#include "gui.h"                //undone
#include "FINDER.h"             //undone

#define FINDER_ICON_SCALE 40    //uncertain
#define SMALL_ICON_SCALE 15     //uncertain

#define ICON_FINDER 0
#define LIST_FINDER 1   

//绘制顶栏按钮
void draw_button(int posX, int posY, int id)
{
	for (int i = 0; i < FINDER_ICON_SCALE; i++)
	{
		for (int j = 0; j < FINDER_ICON_SCALE; j++)
		{
			//uncertain
			draw_point(posX + i, posY + j, finder_icon[id][i][j]);
		}
	}
}

//绘制列表图中缩略图标
void draw_small_icon(int posX, int posY, int type)
{
	for (int i = 0; i < SMALL_ICON_SCALE; i++)
	{
		for (int j = 0; j < SMALL_ICON_SCALE; j++)
		{
			//uncertain
			draw_point(posX + i, posY + j, small_icon[type][i][j]);
		}
	}
}

//绘制正方形
void drawRec(int posX, int posY, int length)
{
	for (int i = 0; i < length; i++)
	{
		draw_point(posX + i, posY, 0x0);
		draw_point(posX + i, posY + length, 0x0);
	}
	for (int j = 0; j < length; j++)
	{
		draw_point(posX, posY + i, 0x0);
		draw_point(posX + length, posY + i, 0x0);
	}
}

//绘制窗口区域背景
void drawbg(int posX, int posY, int height, int color)
{
	for (int i = 1; i < WINDOWWIDTH - 1; i++)
	{
		for (int j = 1; j < height; j++)
		{
			draw_point(posX + i, posY + j, color);
		}
	}
}

//绘制窗口局部背景，如选中图标的背景
void drawbgl(int posX, int posY, int width, int height, int color)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0 ; j < height; j++)
		{
			draw_point(posX + i, posY + j, color);
		}
	}
}

//初始化窗口界面
void InitFolder(window* w)
{
	//绘制背景,uncertain
	drawbg(w->xPos, w->yPos + 20, 60, 0xE0FF);
	drawbg(w->xPos, w->yPos + 80, 300, 0xFFFF);

	draw_button(w->xPos + 20, w->yPos + 30, ICON_GRID);
	draw_button(w->xPos + 80, w->yPos + 30, ICON_LIST);
	draw_button(w->xPos + 340, w->yPos + 30, ICON_ADDFOLDER);
	draw_button(w->xPos + 400, w->yPos + 30, ICON_ADDFILE);
	draw_button(w->xPos + 460, w->yPos + 30, ICON_DELETE);
	draw_button(w->xPos + 520, w->yPos + 30, ICON_UP);

    //uncertain

	//显示窗口名称
	draw_string(w->xPos + 80; w->yPos + 2, "-", 0xFFFF);
	draw_string(w->xPos + 85; w->yPos + 2, w->content->name, 0xFFFF);

	int type = w->wType;
	int focusOne = w->focusIndex;
	int currentX = w->xPos;
	int currentY = w->yPos;
	node* ptr = w->content;
	int count = 1;  //记录是否绘制到聚焦文件（夹）

	switch(type)
	{
		case ICON_FINDER:
		{
			if (ptr != 0)
			{
				ptr = ptr->FirstChild;
				currentX += 10;
				currentY += 80;

				while(!ptr)
				{
					//scale uncertain
					if (currentY + 100 > w->yPos + WINDOWHEIGHT)
						break;
					if (ptr-> type == FILE)
					{
						draw_icon(ICON_FILE, currentX, currentY);
						draw_string(currentX, currentY + 80, ptr->name, 0x0);
					}
					else if (ptr->type == FOLDER)
					{
						draw_icon(ICON_FOLDER, currentX, currentY);
						draw_string(currentX, currentY + 80, ptr->name, 0x0);
					}

					if (count == focusOne)
						drawbgl(currentX, currentY, 80, 90, 0x87CE);
					count++;

					//更新位置信息
					if (currentX + 100 < w->xPos + WINDOWWIDTH)
						currentX += 90;
					else
					{
						currentX = w->xPos + 10;
						currentY += 100;
					}
					ptr = ptr->Brother;
				}
			}
			break;
		}
		case LIST_FINDER:
		{
			if (ptr != 0)
			{
				currentY += 80;
				ptr = ptr->FirstChild;

				draw_string(currentX + 30, currentY + 5, "Name", 0x0);
				draw_string(currentX + 250, currentY + 5, "Type", 0x0);
				draw_string(currentX + 430, currentY + 5, "FixedDate", 0x0);

				currentY += 30;

				while(ptr != 0)
				{
					if (currentY + 30 > w->yPos + WINDOWHEIGHT)
						break;
					if (ptr -> type == FILE)
					{
						draw_small_icon(currentX + 25, currentY + 2, SICON_FILE);
						draw_string(currentX + 40, currentY + 2, ptr->name, 0x0);
						draw_string(currentX + 250, currentY + 2, "File", 0x0);
						draw_string(currentX + 430, currentY + 2, "2014/12/4", 0x0);
					}
					else if (ptr -> type == FOLDER)
					{
						draw_small_icon(currentX + 25, currentY + 2, SICON_FILE);
						draw_string(currentX + 40, currentY + 2, ptr->name, 0x0);
						draw_string(currentX + 250, currentY + 2, "Folder", 0x0);
						draw_string(currentX + 430, currentY + 2, "2014/12/5", 0x0);
					}
					if (count == focusOne)
						//uncertain
						drawbg(currentX, currentY, 20, 0xCDBE);
					count++;

					ptr = ptr->Brother;
					currentY += 20;
				}
			}
			else
			{
				currentY += 80;
				draw_string(currentX + 30, currentY + 5, "Name", 0x0);
				draw_string(currentX + 250, currentY + 5, "Type", 0x0);
				draw_string(currentX + 430, currentY + 5, "FixedDate", 0x0);
			}
			break;
		}
		default:
		{
			return;
		}
	}
}

//获取点击的窗口事件
int WindowGetEvent(int x, int y, int type)
{
	int x2, y2, nx, ny;
	int result;

	switch(type)
	{
		case ICON_FINDER:
		{
			if (y < 20 || y > 380)
				return 0;
			else if (y <= 80)
			{
				if (y <= 30 || y >= 70)
					return 0;

				//点击到Grid_Icon
				if (x > 20 && x < 80)
					return -1;

				//点击到List_Icon
				if (x > 80 && x < 140)
					return -2;

				//点击到AddFolder
				if (x > 340 && x < 400)
					return -3;

				//点击到AddFile
				if (x > 400 && x < 460)
					return -4;

				//点击到删除
				if (x > 460 && x < 520)
					return -5;

				//点击到返回到上一级菜单
				if (x > 520 && x < 580)
					return -6;
				
				return 0;
			}
			else
			{
				if (x < 10 || x > 550) 
					return 0;

				x2 = x - 10;
				y2 = y - 80;
				nx = x2 / 90 + 1;
				ny = y2 / 100;
				result = nx + ny * 6;
				return result;
			}
			break;
		}
		case LIST_FINDER:
		{
			if (y < 20 || y > 380)
				return 0;
			else if (y <= 80)
			{
				if (y <= 30 || y >= 70)
					return 0;

				//点击到Grid_Icon
				if (x > 20 && x < 80)
					return -1;

				//点击到List_Icon
				if (x > 80 && x < 140)
					return -2;

				//点击到AddFolder
				if (x > 340 && x < 400)
					return -3;

				//点击到AddFile
				if (x > 400 && x < 460)
					return -4;

				//点击到删除
				if (x > 460 && x < 520)
					return -5;

				//点击到返回到上一级菜单
				if (x > 520 && x < 580)
					return -6;

				return 0;
			}
			else
			{
				if (y < 110)
					return 0;

				y2 = y - 110;
				ny = y2 / 20 + 1;
				result = ny;
				return result;
			}
		}
	}
}

//窗口处理函数，event为鼠标点击类型，1为单击，3为双击
void Folder(window* w, int x, int y, int event)
{
	int situation = WindowGetEvent(x, y, w->wType);
	switch(situation)
	{
		case 0:
		{
			w->focusIndex = 0;
			break;
		}
		case -1:
		{
			//uncertain 1表示单击事件，后续可替换
			if (event == 1)
				w->wType = ICON;
			w->focusIndex = 0;
			break;
		}
		case -2:
		{
			if (event == 1)
				w->wType = LIST;
			w->focusIndex = 0;
			break;
		}
		case -3:
		{
			if (event == 1)
				Addnode(w->content, "NewFolder", 1, FOLDER);
			w->focusIndex = 0;
			break;
		}
		case -4:
		{
			if (event == 1)
				Addnode(w->content, "NewFile.txt", 1, FILE);
			w->focusIndex = 0;
			break; 
		}
		case -5:
		{
			if (event == 1 && w->focusIndex != 0)
				Deletenode(w->content, w->focusIndex);
			w->focusIndex = 0;
			break;
		}
		case -6:
		{
			if (event == 1 && w->content->Parent != 0)
				w->content = w->content->Parent;
			w->focusIndex = 0;
			break;
		}
		default:
		{
			if (situation <= 0)
				break;
			//判断单双击事件
			if (event == 1)
			{
				w->focusIndex = situation;
				break;
			}
			//event = 3为双击事件
			else if (event == 3)
			{
				node* newNode = Getnode(w->content, situation);
				if (newNode != 0 && newNode->type == FOLDER)
				{
					w->content = newNode;
					w->focusIndex = 0;
				}
				else
				{
					w->focusIndex = situation;
				}
				break;
			}
		}
	}

	InitFolder(w);
	display_to_screen(w->xPos, w->yPos, WINDOWWIDTH, WINDOWHEIGHT);
}


