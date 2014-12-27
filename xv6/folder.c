#include "window.h"
#include "FINDER.h"
#include "types.h"
#include "defs.h"
#include "gui.h"

#define ICON_FINDER 0
#define LIST_FINDER 8

void draw_button(unsigned int x, unsigned int y, int id)
{
	int i, j;
	for(i = 0; i < 40; i++)
	{
		for(j = 0; j < 40; j++)
		{
			if(finder_icon[id][i][j] != 2016)
				draw_point(x + i, y + j, finder_icon[id][i][j]);
		}
	}
}

void draw_small_icon(unsigned int x, unsigned int y, int type)
{
	int i, j;
	for(i = 0; i < 15; i++)
	{
		for(j = 0; j < 15; j++)
		{
			if(small_icon[type][i][j] != 2016)
				draw_point(x + i, y + j, small_icon[type][i][j]);
		}
	}
}

void drawRec(int px, int py, int len)
{
	int i, j;
	for(i = 0;i < len; i++)
	{
		draw_point(px+i,py,0x0);
		draw_point(px+i,py+len,0x0);
	}

	for(j = 0;j < len; j++)
	{
		draw_point(px,py+j,0x0);
		draw_point(px+len,py+j,0x0);
	}
}


void drawbg(int px, int py, int height, int color)
{
	int i, j;
	for(i = 0; i < WindowWidth-2; i++)
		for(j = 0; j < height; j++)
		{
			draw_point(px+i, py+j, color);
		}
}

void drawbgl(int px, int py, int width, int height, int color)
{
	int i, j;
	for(i = 0; i < width; i++)
		for(j = 0; j < height; j++)
		{
			draw_point(px+i, py+j, color);
		}
}

void InitFolder(struct Window* wd)
{
	int i, j;
	for(i = 1; i < WindowWidth - 1; i++)
	{
		for(j = 20; j < WindowHeight - 20; j++)
		{
			if(j < 79)
			{
				draw_point(i + wd->Pos_x, j + wd->Pos_y, 0xDF7C);
			}else if(j == 79){
				draw_point(i + wd->Pos_x, j + wd->Pos_y, 0x7CAC);
			}else if(j < 380){
				draw_point(i + wd->Pos_x, j + wd->Pos_y, 0xFFFF);
			}
		}
	}
	
	draw_button(wd->Pos_x+20,wd->Pos_y+30,ICON_GRID);
	draw_button(wd->Pos_x+80,wd->Pos_y+30,ICON_LIST);
	draw_button(wd->Pos_x+420,wd->Pos_y+30,ICON_UP);
	draw_button(wd->Pos_x+480,wd->Pos_y+30,ICON_ADDFOLDER);
	draw_button(wd->Pos_x+540,wd->Pos_y+30,ICON_ADDFILE);
	draw_button(wd->Pos_x+600,wd->Pos_y+30,ICON_DELETE);

	int cur_x = wd->Pos_x;
	int cur_y = wd->Pos_y;
	int type = wd->Cur_icon;
	int ChoosenOne = wd->ChoosenOne;

	for(i = 88; i < WindowWidth-1;i++)
		for(j = 1; j < 20;j++)
			draw_point(i + wd->Pos_x, j + wd->Pos_y, 0x5ACB);

	draw_string(wd->Pos_x + 80, wd->Pos_y + 2, ":", 0xFFFF);
	draw_string(wd->Pos_x + 88, wd->Pos_y + 2, wd->Cur_Node->Name, 0xFFFF);


	switch(type)
	{
		case ICON_FINDER:
		{
			cur_x = wd->Pos_x + 15;
			cur_y = wd->Pos_y + 80;

			struct Node* ptr;
			ptr = wd->Cur_Node;
			ptr = ptr->Firstchild;

			int flag = 1;

			while(ptr != 0)
			{
				if(cur_y + 100 > wd->Pos_y + WindowHeight)
					break;

				if(flag == ChoosenOne) drawbgl(cur_x,cur_y,75,90,0x867D);
				flag++;
				if(ptr->type == 0)
				{
					draw_icon(ICON_FOLDER, cur_x, cur_y);
					draw_string(cur_x+20, cur_y+75, ptr->Name, 0x0);

				}else if(ptr->type == 1)
				{
					draw_icon(ICON_FILE, cur_x, cur_y);
					draw_string(cur_x, cur_y+75, ptr->Name, 0x0);
				}

				if(cur_x + 180 < wd->Pos_x + WindowWidth)			
					cur_x += 90;
				else{
					cur_x = wd->Pos_x + 15;
					cur_y += 100;
				}
				ptr = ptr->Brother;

			}
			break;
		}
		case LIST_FINDER:
		{
			cur_x = wd->Pos_x;
			cur_y = wd->Pos_y + 80;
			struct Node* ptr;
			ptr = wd->Cur_Node;
			ptr = ptr->Firstchild;

			draw_string(cur_x+30, cur_y+10, "Name", 0x0);
			draw_string(cur_x+250, cur_y+10, "ChangeDate", 0x0);
			draw_string(cur_x+430, cur_y+10, "Type", 0x0);
			draw_string(cur_x+550, cur_y+10, "Creator", 0x0);

			int i;
			for(i = 0; i < WindowWidth-1; i++)
				draw_point(cur_x+i,cur_y+40,0x0);
			cur_y += 40;

			int flag = 1;

			while(ptr != 0)
			{
				if(cur_y + 20 > wd->Pos_y + WindowHeight-20)
					break;
				if(ptr->type == 0)
				{
					if(flag%2) drawbg(cur_x+1,cur_y,20,0xDE9C);
					if(flag == ChoosenOne) drawbg(cur_x+1,cur_y,20,0x867D);
					flag++;
					draw_small_icon(cur_x+20, cur_y+2, SICON_FOLDER);
					draw_string(cur_x+40, cur_y+2, ptr->Name, 0x0);
					draw_string(cur_x+250, cur_y+2, "2014/12/27 00:00", 0x0);
					draw_string(cur_x+430, cur_y+2, "Document", 0x0);
					draw_string(cur_x+550, cur_y+2, "WangChengpeng", 0x0);
				}else if(ptr->type == 1)
				{
					if(flag%2) drawbg(cur_x+1,cur_y,20,0xDE9C);
					if(flag == ChoosenOne) drawbg(cur_x+1,cur_y,20,0x867D);
					flag++;
					draw_small_icon(cur_x+20, cur_y+2, SICON_FILE);
					draw_string(cur_x+40, cur_y+2, ptr->Name, 0x0);
					draw_string(cur_x+250, cur_y+2, "2013/12/27 00:00", 0x0);
					draw_string(cur_x+430, cur_y+2, "File", 0x0);
					draw_string(cur_x+550, cur_y+2, "WangChengpeng", 0x0);
				}
				cur_y += 20;
				ptr = ptr->Brother;
			}
			break;
		}
		default:
		{
			return;
			break;
		}
	}
}

int WindowGetEvent(int px, int py, int type)
{
	switch(type)
	{
		case ICON_FINDER:
		{
			if(py <= 20 || py >= 380) 
				return 0;
			else if(py <= 80)
			{
				if(py <= 30 || py >= 70) 
					return 0;
				else
				{
					if(px > 20 && px < 60) return -1;
					if(px > 80 && px < 120) return -2;
					if(px > 420 && px < 460) return -6;
					if(px > 480 && px < 520) return -3;
					if(px > 540 && px < 580) return -4;
					if(px > 600 && px < 640) return -5;
					return 0;
				}
			}
			else
			{
				if(px <= 15 || px >= 645) return 0;
				int newx = px - 15;
				int newy = py - 80;

				int ix = (newx / 90) + 1;
				int iy = (newy / 100);

				int result = ix + (iy * 7);
				return result;
			}
			break;
		}
		case LIST_FINDER:
		{
			if(py <= 20 || py >= 380) 
				return 0;
			else if(py <= 80)
			{
				if(py <= 30 || py >= 70) 
					return 0;
				else
				{
					if(px > 20 && px < 60) return -1;
					if(px > 80 && px < 120) return -2;
					if(px > 420 && px < 460) return -6;
					if(px > 480 && px < 520) return -3;
					if(px > 540 && px < 580) return -4;
					if(px > 600 && px < 640) return -5;
					return 0;
				}
			}
			else
			{
				if(py <= 120) return 0;
				int newy = py - 120;
				int iy = (newy / 20) + 1;
				return iy;
			}
			break;
		}
		default:
		{
			return 0;
			break;
		}
	}
	return 0;
}

void Folder(struct Window* wd, int px, int py, int event)
{
	//cprintf("icon is %d\n", wd->Cur_icon);
	int flag = WindowGetEvent(px,py,wd->Cur_icon);
	
	switch(flag)
	{
		case 0://nothing
		{
			wd->ChoosenOne = 0;
			break;
		}
		case -1://ICON mode
		{
			wd->ChoosenOne = 0;
			if(event == 1) wd->Cur_icon = ICON_FINDER;
			break;
		}
		case -2://LIST mode
		{
			wd->ChoosenOne = 0;
			if(event == 1) wd->Cur_icon = LIST_FINDER;
			break;
		}
		case -3://New folder
		{
			wd->ChoosenOne = 0;
			if(event == 1) Add_Node(wd->Cur_Node,"folder",0);
			break;
		}
		case -4://New file
		{
			wd->ChoosenOne = 0;
			if(event == 1) Add_Node(wd->Cur_Node,"newfile.txt",1);
			break;
		}
		case -5://delete
		{
			if(event == 1) Delete_Node(wd->Cur_Node, wd->ChoosenOne);
			wd->ChoosenOne = 0;
			break;
		}
		case -6://up
		{
			if(event == 1 && wd->Cur_Node->Parent != 0) 
				wd->Cur_Node = wd->Cur_Node->Parent;
			wd->ChoosenOne = 0;
			break;
		}
		default:
		{
			if(flag <= 0) break;
			if(event == 1){
				//cprintf("%d node is choosen\n",event,flag);
				wd->ChoosenOne = flag;
			}else if(event == 3){
				struct Node* curNode = GetNode(wd->Cur_Node, flag);
				if(curNode != 0 && curNode->type == 0)
				{
					wd->Cur_Node = curNode;
					wd->ChoosenOne = 0;
				}
				else wd->ChoosenOne = 0; 
			}
			break;
		}
	}
	InitFolder(wd);
	display_to_screen(wd->Pos_x, wd->Pos_y, WindowWidth, WindowHeight);
}