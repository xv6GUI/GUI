#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
#include "window.h"

///////////////////////////////////////////////////////////functions of node//////////////////////////////////////////////////
char* strcpy(char *s, char *t)
{
    char *temp;
    temp = s;

    while((*s++ = *t++) != 0);

    return temp;
}

void InitNode()
{
	NumOfNode = 0;

	//初始化结点类型
	int i;
	for(i = 0; i < MaxNode; i++)
		node[i].NodeType = -1;

	Root = RequireNode();
	InfoFile = RequireNode();

	//初始化根结点,添加一个文件和文件夹
	strcpy(Root->Name,"Root");
	Root->NodeType = 0;
	Root->Parent = 0;
	Root->Firstchild = InfoFile;
	Root->Lastchild = InfoFile;
	Root->Brother = 0;
	
	strcpy(InfoFile->Name,"Info.doc");
	InfoFile->NodeType = 1;
	InfoFile->Parent = Root;
	InfoFile->Firstchild = 0;
	InfoFile->Lastchild = 0;
	InfoFile->Brother = 0;
}

struct Node* RequireNode()
{
	if(MaxNode <= NumOfNode)
		return 0;

	int i;
	for(i = 0; i < MaxNode; i++)
	{
		if(node[i].NodeType == -1)
			break;
	}

	if (i == MaxNode)
		return 0;
	else
	{
		NumOfNode++;
		node[i].NodeType = 0;
		return (&node[i]);
	}
}

void ReleaseNode(struct Node* p)
{
	p->Parent = 0;
	p->Firstchild = 0;
	p->Lastchild = 0;
	p->Brother = 0;
	p->NodeType = -1;
	NumOfNode--;
}

int Add_Node(struct Node* currentNode, char* newName, int newType)
{
	struct Node* p;

	if(currentNode->NodeType)
		return -1;
	else if(currentNode->Firstchild == 0)
	{
		p = RequireNode();

		if(p == 0) 
			return -2;

		p->Parent = currentNode;
		currentNode->Firstchild = p;
		currentNode->Lastchild = p;
		
		p->NodeType = newType;
		p->Firstchild = 0;
		p->Lastchild = 0;
		p->Brother = 0;
		strcpy(p->Name, newName);
		return 0;
	}
	else
	{
		p = RequireNode();

		if(p == 0) 
			return -3;

		p->Parent = currentNode;
		p->Firstchild = 0;
		p->Lastchild = 0;
		p->Brother = 0;
		strcpy(p->Name, newName);
		p->NodeType = newType;

		currentNode->Lastchild->Brother = p;
		currentNode->Lastchild = p;
		return 0;
	}
}

int Delete_Node(struct Node* p, int n)
{
	if(n < 1) 
		return -1;

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

	if(s == 0) 
		return -1;

	if(t == ptr)
	{
		ptr->Firstchild = s->Brother;
		t = 0;
	}
	else
		t->Brother = s->Brother;

	if(s == ptr->Lastchild)
		ptr->Lastchild = t;

	ReleaseNode(s);
	return 0;
}

int Remove_Node(struct Node* content, struct Node* old)
{
	if(content->NodeType)
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
		newNode->NodeType = old->NodeType;
		if(newNode->NodeType)
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
		newNode->NodeType = old->NodeType;
		content->Lastchild->Brother = newNode;
		content->Lastchild = newNode;
		if(newNode->NodeType)
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

void Rename_Node(struct Node* p, char* newname)
{
	strcpy(p->Name, newname);
}

struct Node* GetNode(struct Node* p, int n)
{
	if(n < 1) 
		return 0;

	int num = 1;
	struct Node* ptr = p;
	struct Node* s = ptr->Firstchild;
	
	while(num < n && s != 0)
	{
		num++;
		s = s->Brother;
	}

	return s;
}

//////////////////////////////////////////////////////functions of window//////////////////////////////////////////////

void InitWindow()
{
	int i;
	for(i = 0; i < MaxWindow; i++)
		window[i].WindowType = -1;
	NumOfWindow = 0;

	WindowLine = RequireWindow();
	WindowLine->WindowType = 0;
	WindowLine->FocusOne = 0;

	WindowLine->ContentNode=0;
	WindowLine->next = 0;
	WindowLine->pre = 0;

	WindowLine->Pos_x =0;
	WindowLine->Pos_y = 0;
}

struct Window* RequireWindow()
{
	if(MaxWindow <= NumOfWindow)
		return 0;

	int i;
	for(i = 0; i < MaxWindow; i++)
	{
		if(window[i].WindowType == -1)
			break;
	}

	if (i == MaxWindow)
		return 0;
	else
	{
		NumOfWindow++;
		return &window[i];
	}
}

void ReleaseWindow(struct Window* w)
{
	NumOfWindow--;
	w->WindowType = -1;
	w->ContentNode = 0;

	w->pre = 0;
	w->next = 0;
	
	w->Pos_x = 0;
	w->Pos_y = 0;
	w->FocusOne = 0;
}

struct Window* Add_Window(int icon)
{
	struct Window* nw = RequireWindow();

	if(!nw) 
		return 0;

	nw->ContentNode = Root;
	nw->FocusOne = 0;
	nw->WindowType = icon;

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

		//保持窗口间的间距，方便窗口选择点击
		nw->Pos_x = 30 + ptr->Pos_x;
		nw->Pos_y = 30 + ptr->Pos_y;
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

void Set_Window_Node(struct Window* w, struct Node* node)
{
	if (w == 0)
		return;
	if (node->NodeType == 1)
		return;

	if (node->NodeType == 0)
		w->ContentNode = node;
}

void Set_Window_Icon(struct Window* w, int type)
{
	if (w == 0)
		return;
	if (type < 0)
		return;

	w->WindowType = type;
}

void Focus(struct Window* aim)
{
	if(WindowLine->next == aim) 
		return;

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

struct Window* Get_LastWindow()
{
	struct Window* p;
	struct Window* q;
	for (p = WindowLine->next, q = p; p != 0; p = p->next)
	{
		q = p;
	}
	return q;
}

struct Window* Click_Get_Window(int px, int py)
{
	int x, y;
	struct Window* ptr = WindowLine->next;

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
	
	for (ptr = WindowLine->next; ptr != 0; ptr = ptr->next)
	{
		if (ptr->WindowType == type)
			break;
	}
	return ptr;
}
