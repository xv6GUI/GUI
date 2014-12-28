#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
#include "window.h"

////////////////////////////////////////functions of node//////////////////////////////////////////////////
char* strcpy(char *s, char *t)
{
    char *s1;
    s1 = s;
    while((*s++ = *t++) != 0);
    return s1;
}

void InitNode()
{
	int i;

	//初始化结点类型
	for(i = 0; i < MaxNode; i++)
	{
		node[i].type = -1;
	}

	NumOfNode = 0;

	Root = RequireNode();
	User_folder = RequireNode();
	Readme_file = RequireNode();

	//初始化根结点
	strcpy(Root->Name,"Root");
	Root->type = 0;
	Root->Parent = 0;
	Root->Firstchild = User_folder;
	Root->Lastchild = Readme_file;
	Root->Brother = 0;
	
	strcpy(Readme_file->Name,"Info.txt");
	Readme_file->type = 1;
	Readme_file->Parent = Root;
	Readme_file->Firstchild = 0;
	Readme_file->Lastchild = 0;
	Readme_file->Brother = 0;

	strcpy(User_folder->Name,"Index");
	User_folder->type = 0;
	User_folder->Parent = Root;
	User_folder->Firstchild = 0;
	User_folder->Lastchild = 0;
	User_folder->Brother = Readme_file;
}

struct Node* RequireNode()
{
	int i;
	if(MaxNode <= NumOfNode)
		return 0;

	for(i = 0; i < MaxNode; i++)
	{
		if(node[i].type == -1)
		{
			node[i].type = 0;
			NumOfNode++;
			return (&node[i]);
		}
	}
	return 0;
}

void ReleaseNode(struct Node* n)
{
	n->Parent = 0;
	n->Firstchild = 0;
	n->Lastchild = 0;
	n->Brother = 0;
	n->type = -1;
	NumOfNode--;
}

int Add_Node(struct Node* currentNode, char* newName, int newType)
{
	if(currentNode->type)
		return -1;
	else if(currentNode->Firstchild == 0)
	{
		struct Node* p = RequireNode();
		if(!p) 
			return -2;
		p->Parent = currentNode;
		currentNode->Firstchild = p;
		currentNode->Lastchild = p;
		p->Brother = 0;
		p->Firstchild = 0;
		p->Lastchild = 0;
		strcpy(p->Name, newName);
		p->type = newType;
		return 0;
	}
	else
	{
		struct Node* p = RequireNode();
		if(!p) 
			return -3;
		p->Parent = currentNode;
		p->Firstchild = 0;
		p->Lastchild = 0;
		p->Brother = 0;
		strcpy(p->Name, newName);
		p->type = newType;
		currentNode->Lastchild->Brother = p;
		currentNode->Lastchild = p;
		return 0;
	}
}

int Delete_Node(struct Node* p, int n)
{
	if(n < 1) return -1;
	struct Node* ptr = p;
	struct Node* t = ptr;
	struct Node* s = ptr->Firstchild;
	int count = 1;
	while(count < n && s != 0)
	{
		t = s;
		s = s->Brother;
		count++;
	}

	if(s == 0) return -1;
	if(t == ptr)
	{
		ptr->Firstchild = s->Brother;
		t = 0;
	}else
	{
		t->Brother = s->Brother;
	}

	if(s == ptr->Lastchild)
	{
		ptr->Lastchild = t;
	}

	ReleaseNode(s);
	return 0;
}

void Rename_Node(struct Node* p, char* newname)
{
	strcpy(p->Name,newname);
}

struct Node* GetNode(struct Node* p, int n)
{
	if(n < 1) 
		return 0;

	struct Node* ptr = p;
	struct Node* s = ptr->Firstchild;
	int count = 1;
	while(count < n && s != 0)
	{
		s = s->Brother;
		count++;
	}

	return s;
}

int Remove_Node(struct Node* content, struct Node* old)
{
	if(content->type)
		return -1;
	else if(content->Firstchild == 0)
	{
		struct Node* newNode = RequireNode();
		if(!newNode) 
			return -2;
		content->Firstchild = newNode;
		content->Lastchild = newNode;
		newNode->Brother = 0;
		newNode->Parent = content;
		newNode->Firstchild = 0;
		newNode->Lastchild = 0;
		strcpy(newNode->Name, old->Name);
		newNode->type = old->type;
		if(newNode->type)
			return 0;
		else
		{
			struct Node* ptr = old->Firstchild;
			while (ptr != 0)
			{
				Remove_Node(newNode, ptr);
				ptr = ptr->Brother;
			}
			return 0;
		}
	}
	else
	{
		struct Node* newNode = RequireNode();
		if(!newNode) return -2;
		newNode->Brother = 0;
		newNode->Parent = content;
		newNode->Firstchild = 0;
		newNode->Lastchild = 0;
		strcpy(newNode->Name, old->Name);
		newNode->type = old->type;
		content->Lastchild->Brother = newNode;
		content->Lastchild = newNode;
		if(newNode->type)
			return 0;
		else
		{
			struct Node* ptr = old->Firstchild;
			while (ptr != 0)
			{
				Remove_Node(newNode, ptr);
				ptr = ptr->Brother;
			}
			return 0;
		}
	}
}
////////////////////////////////////////////functions of window//////////////////////////////////////////////

void InitWindow()
{
	int i;
	for(i = 0; i <MaxWindow; i++)
	{
		window[i].Cur_icon = -1;
	}

	WindowLine = RequireWindow();
	WindowLine->Cur_icon = 0;
	WindowLine->Cur_Node=0;
	WindowLine->next = 0;
	WindowLine->Pos_x =0;
	WindowLine->Pos_y = 0;
	WindowLine->pre = 0;
	WindowLine->ChoosenOne = 0;
	NumOfWindow = 0;
}

struct Window* RequireWindow()
{
	if(MaxWindow <= NumOfWindow)
		return 0;

	int i;
	for(i = 0; i < MaxWindow; i++)
	{
		if(window[i].Cur_icon == -1)
		{
			NumOfWindow++;
			return (&window[i]);
		}
	}
	return 0;
}

void ReleaseWindow(struct Window* w)
{
	NumOfWindow--;
	w->Cur_icon = -1;
	w->Cur_Node = 0;

	w->pre = 0;
	w->next = 0;
	
	w->Pos_x = 0;
	w->Pos_y = 0;
	w->ChoosenOne = 0;
}

struct Window* Add_Window(int icon)
{
	struct Window* nw = RequireWindow();
	if(!nw) return 0;

	nw->Cur_icon = icon;
	nw->Cur_Node = Root;
	nw->ChoosenOne = 0;

	if(WindowLine->next == 0)
	{
		WindowLine->next = nw;
		nw->next = 0;
		nw->pre = 0;
		nw->Pos_x = 20;
		nw->Pos_y = 20;
	}
	else
	{
		struct Window* ptr = WindowLine->next;
		nw->pre = 0;
		nw->next = ptr;
		ptr->pre = nw;
		WindowLine->next = nw;

		if(ptr->Pos_x == 100)
		{
			nw->Pos_x = 20;
			nw->Pos_y = 20;
		}
		else
		{
			nw->Pos_x = ptr->Pos_x + 20;
			nw->Pos_y = ptr->Pos_y + 20;
		}
	}
	return nw;
}

void Close_Window()
{
	if(WindowLine->next == 0) 
		return;

	struct Window* ptr = WindowLine->next;
	struct Window* p = ptr->next;
	if(p == 0)
	{
		ReleaseWindow(ptr);
		WindowLine->next = 0;
	}
	else
	{
		ReleaseWindow(ptr);
		WindowLine->next = p;
		p->pre = 0;
	}
}

void Move_Window(struct Window* aim, int px, int py)
{
	if (!aim)
	{
		aim->Pos_x += px;
		aim->Pos_y += py;
	}
}

void Focus(struct Window* aim)
{
	if(WindowLine->next == aim) 
		return;

	struct Window* pre = aim->pre;
	pre->next = aim->next;
	if(aim->next != 0)
	{
		aim->next->pre = pre;
	}
	struct Window* old = WindowLine->next;
	WindowLine->next = aim;
	aim->next = old;
	old ->pre = aim;
	aim->pre = 0;
}

struct Window* Get_LastWindow()
{
	struct Window* p = WindowLine->next;
	struct Window* q = p;
	while(p != 0)
	{
		q = p;
		p = p->next;
	}
	return q;
}

struct Window* Click_Get_Window(int px, int py)
{
	struct Window* ptr = WindowLine->next;
	int x, y;
	while(ptr)
	{
		x = ptr->Pos_x;
		y = ptr->Pos_y;
		if (px <= x + WindowWidth && px >= x && py <= y + WindowHeight && py >= y)
			break;
		ptr = ptr->next;
	}
	return ptr;
}

struct Window* get_window_by_icon(int type)
{
	struct Window* ptr = WindowLine->next;
	while(ptr)
	{
		if(ptr->Cur_icon == type)
			return ptr;
		ptr = ptr->next;
	}
	return 0;
};

void Set_Window_Node(struct Window* wd, struct Node* node)
{
	if(wd && node)
		wd->Cur_Node = node;
}

void Set_Window_Icon(struct Window* wd, int type)
{
	if(type >= 0 && wd != 0)
		wd->Cur_icon = type;
}
