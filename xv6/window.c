#include <cstdlib>
#include "window.h"
//using namespace std;

///////////////////////////////////////////////////////////file//////////////////////////////////////////////////////////////////////

//初始化内存
//分配空间给root
void Initnode()
{
	AvailableMemory = MAXNODE;
	for (int i = 0; i < MAXNODE; i++)
	{
		memory[i].name = 0;
		memory[i].type = UNCERTAIN;
		memory[i].childrenNum = 0;
		memory[i].Parent = 0;
		memory[i].FirstChild = 0;
		memory[i].Brother = 0;
	}

	AvailableMemory--;
	memory[0].name = "Root";
	memory[0].type = FOLDER;

	root = &memory[0];
}

//申请一个结点类型为nodeType的文件或者文件夹
//返回指向申请后的内存空间的指针
node* Newnode(int nodeType)
{
	if (AvailableMemory == 0 || (nodeType != 1 && nodeType != 2))
		return 0;

	int i = 0;
	while (memory[i].type != UNCERTAIN)
		i++;
	memory[i].type = nodeType;
	AvailableMemory--;

	node* p = &memory[i];
	return p;
}

//释放r指向的内存空间
//AvailableMemory++
void Freenode(node* r)
{
	r->name = 0;
	r->type = UNCERTAIN;
	r->childrenNum = 0;
	r->Parent = 0;
	r->FirstChild = 0;
	r->Brother = 0;
	AvailableMemory++;
}

//在parent目录下新建文件或者文件夹
//名称为ch，作为parent的第location个孩子
int Addnode(node* parent, char* ch, int location, int nodeType)
{
	if (parent->type != FOLDER)
		return -1;

	if (parent->childrenNum < location - 1)
		return -2;

	if (nodeType != FILE && nodeType != FOLDER)
		return -3;

	node* r = Newnode(nodeType);
	if (!r)
		return -4;

	r->name = ch;
	r->Parent = parent;
	
	node* t = parent->FirstChild;
	for (int i = 1; i < location; i++)
		t = t->Brother;
	r->Brother = t;

	parent->childrenNum++;
	if (location == 1)
		parent->FirstChild = r;
	else
	{
		t = parent->FirstChild;
		for (int i = 1; i < location - 1; i++)
			t = t->Brother;
		t->Brother = r;
	}
	return 0;
}

//删除parent目录下的第location个孩子
//如调用出现异常返回-1、-2，否则返回0
int Deletenode(node* parent, int location)
{
	if (parent->type != FOLDER)
		return -1;
	if (location > parent->childrenNum || location < 1)
		return -2;

	node* t = parent->FirstChild;
	for (int i = 1; i < location; i++)
		t = t->Brother;
	
	if (location > 1)
	{
		node* r = parent->FirstChild;
		for (int i = 1; i < location - 1; i++)
			r = r->Brother;
		r->Brother = t->Brother;
	}
	else //location = 1
	{
		parent->FirstChild = t->Brother;
	}
	parent->childrenNum--;

	Freenode(t);
	return 0;
}

//将结点r的name置为name
void Renamenode(node* r, char* name)
{
	r->name = name;
}

//得到parent的第location个结点
//返回指向该结点的指针
node* Getnode(node* parent, int location)
{
	if (parent->childrenNum < location)
		return 0;

	node* t = parent->FirstChild;
	for (int i = 1; i < location; i++)
		t = t->Brother;

	return t;
}

//将r复制到parent目录下,且作为parent的第一个结点
//若出现异常，返回-1，否则返回0
int Removenode(node* parent, node* r)
{
	if (parent->type != FOLDER)
		return -1;

	node* t = Newnode(r->type);
	t->name = r->name;
	t->childrenNum = r->childrenNum;
	t->Parent = parent;
	t->FirstChild = r->FirstChild;

	parent->childrenNum++;
	if (parent->FirstChild == 0)
	{
		parent->FirstChild = t;
		t->Brother = 0;
	}
	else
	{
		t->Brother = parent->FirstChild;
		parent->FirstChild = t;
	}

	if (t->type == FILE)
		return 0;
	else
	{
		node* ptr = r->FirstChild;
		while (ptr != 0)
		{
			Removenode(t, ptr);
			ptr = ptr->Brother;
		}
	}
	return 0;
}

///////////////////////////////////////////////////window/////////////////////////////////////////////////////////////////////////////
//初始化内存
//为窗口队列的头指针分配一块内存
void InitWindow()
{
	for (int i = 0; i < MAXWINDOW; i++)
	{
		windowMemory[i].wType = -1;
		windowMemory[i].xPos = 0;
		windowMemory[i].yPos = 0;
		windowMemory[i].content = 0;
		windowMemory[i].contentNum = 0;
		windowMemory[i].focusIndex = 0;
		windowMemory[i].pre = 0;
		windowMemory[i].next = 0;
	}
	AvailableWindowMemory = MAXWINDOW;

	windowHead = NewWindow(0);
}

//申请类型为type的窗口
//返回值为窗口的指针
window* NewWindow(int type)
{
	if (AvailableWindowMemory <= 0 || (type != ICON && type != LIST))
		return 0;

	int i;
	for (i = 0; i < MAXWINDOW; i++)
	{
		if (windowMemory[i].wType == -1)
			break;
	}
	windowMemory[i].wType = type;
	AvailableWindowMemory--;
	return (&windowMemory[i]);
}

//释放w指向的窗口所在的内存空间
void FreeWindow(window* w)
{
	w->wType = -1;
	w->xPos = 0;
	w->yPos = 0;
	w->content = 0;
	w->contentNum = 0;
	w->focusIndex = 0;
	w->pre = 0;
	w->next = 0;
	AvailableWindowMemory++;
}

//添加窗口，新建窗口的类型为type
//加入到窗口队列的队首
int AddWindow(int type)
{
	if (type != ICON && type != LIST)
		return -1;
	if (AvailableWindowMemory <= 0)
		return -2;

	window* w = NewWindow(type);
	if (!w)
		return -3;

	if (windowHead->next == 0)
	{
		windowHead->next = w;
		w->pre = windowHead;
		w->xPos = 0;
		w->yPos = 0;
	}
	else
	{
		w->next = windowHead->next;
		windowHead->next = w;
		w->pre = windowHead;
		w->next->pre = w;

		w->xPos = w->next->xPos + 20;
		w->yPos = w->next->yPos + 20;
	}
	return 0;
}

//关闭队首窗口
int CloseWindow()
{
	if (windowHead->next == 0)
		return -1;

	window *w = windowHead->next;
	windowHead->next = w->next;
	if (w->next)
		w->next->pre = windowHead;

	FreeWindow(w);
	return 0;
}

//得到队列中第location个窗口的指针
window* GetWindow(int location)
{
	int index = 1;
	window* ptr = windowHead->next;
	while(ptr != NULL && index < location)
	{
		ptr = ptr->next;
		index++;
	}
	return ptr;
}

//将w放到队列的队首，设置为聚焦
int FocusWindow(window* w)
{
	if (!w)
		return -1;

	window* pre = w->pre;

	if (!pre)
		return -2;

	pre->next = w->next;
	if (w->next)
	{
		w->next->pre = pre;
	}

	window* t = windowHead->next;
	windowHead->next = w;
	w->next = t;
	t->pre = w;
	w->pre = windowHead;
	return 0;
}

//将窗口w沿x方向移动dx，沿y方向移动dy
int MoveWindow(window* w, int dx, int dy)
{
	if (!w)
		return -1;
	w->xPos += dx;
	w->yPos += dy;
	return 0;
}

//返回鼠标点击（x,y)时点中的窗口的指针
window* ClickWindow(int x, int y)
{
	window* ptr;
	for (ptr = windowHead->next; ptr != 0; ptr = ptr->next)
	{
		if (x >= ptr->xPos && x <= ptr->xPos + WINDOWWIDTH && y >= ptr->yPos && y <= ptr->yPos + WINDOWHEIGHT)
			break;
	}
	return ptr;
}

//设置窗口w的类型为type
int setWindowType(window* w, int type)
{
	if (!w)
		return -1;
	if (type < 0)
		return -2;

	w->wType = type;
	return 0;
}

//设置窗口w的内容为r指向的链表
int setWindowContent(window* w, node* r)
{
	if (!w)
		return -1;
	
	w->content = r;
	return 0;
}

//int main(int argc, char* argv[])
//{
//	////////////////////////test node module///////////////////////////
//	Initnode();
//	Addnode(root, "Folder1", 1, FOLDER);
//	Addnode(root, "Folder2", 2, FOLDER);
//	Addnode(root, "Folder3", 3, FOLDER);
//	Deletenode(root, 3);
//	
//	Addnode(Getnode(root, 2), "File1", 1, FILE);
//	Renamenode(Getnode(root, 1), "Folder3");
//
//	Removenode(Getnode(root, 1), Getnode(Getnode(root, 2), 1));
//
//	///////////////////////test window module///////////////////////////
//	InitWindow();
//	AddWindow(1);
//	AddWindow(2);
//	CloseWindow();
//	AddWindow(3);
//	FocusWindow(GetWindow(2));
//	setWindowContent(GetWindow(1), root);
//
//	system("pause");
//	return 0;
//}