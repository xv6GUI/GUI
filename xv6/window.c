#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
#include "window.h"

char*
strcpy(char *s, char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}
////////////////////////////////////////Function of Node//////////////////////////////////////////////////

void ReleaseNode(struct Node* aim)
{
	aim->type = -1;
	aim->Firstchild = 0;
	aim->Lastchild = 0;
	aim->Brother = 0;
	aim->Parent = 0;
	NumOfNode--;
}

struct Node* RequireNode()
{
	int i;
	if(NumOfNode >= MaxNode)
		return 0;
	for(i = 0; i < MaxNode; i++)
	{
		if(node[i].type == -1)
		{
			node[i].type = 0;
			NumOfNode++;
			return &node[i];
		}
	}
	return 0;
}

void InitNode()
{
	int i;
	for(i = 0; i < MaxNode; i++)
	{
		node[i].type = -1;
	}

	NumOfNode = 0;

	Root = RequireNode();
	User_folder = RequireNode();
	Readme_file = RequireNode();

	strcpy(Root->Name,"Root");
	Root->type = 0;
	Root->Parent = 0;
	Root->Brother = 0;
	Root->Firstchild = User_folder;
	Root->Lastchild = Readme_file;

	strcpy(User_folder->Name,"User");
	User_folder->type = 0;
	User_folder->Parent = Root;
	User_folder->Brother = Readme_file;
	User_folder->Firstchild = 0;
	User_folder->Lastchild = 0;

	strcpy(Readme_file->Name,"Readme.txt");
	Readme_file->type = 1;
	Readme_file->Parent = Root;
	Readme_file->Brother = 0;
	Readme_file->Firstchild = 0;
	Readme_file->Lastchild = 0;
}

void Nodels(struct Node* currentNode)
{
	struct Node* firstNode = currentNode->Firstchild;
	if(firstNode == 0)
		return;
	struct Node* tmp = firstNode;
	while(1){
		cprintf("%s\n",tmp->Name);
		tmp = tmp->Brother;
		if(tmp == 0)
			break;
	}
}

void Display(struct Node* Cur)
{
	cprintf("the type of node: ");
	if(Cur->type) cprintf("file\n");
	else cprintf("folder\n");
	cprintf("the name of node: %s\n", Cur->Name);

	if(Cur->Firstchild)
		cprintf("the name of firstchild: %s\n", Cur->Firstchild->Name);
	if(Cur->Lastchild)
		cprintf("the name of lastchild: %s\n", Cur->Lastchild->Name);
}

int Add_Node(struct Node* currentNode, char* newName, int newType)
{
	if(currentNode->type)
		return -1;
	else if(currentNode->Firstchild == 0){
		struct Node* newNode = RequireNode();
		if(!newNode) return -2;
		currentNode->Firstchild = newNode;
		currentNode->Lastchild = newNode;
		newNode->Brother = 0;
		newNode->Parent = currentNode;
		newNode->Firstchild = 0;
		newNode->Lastchild = 0;
		strcpy(newNode->Name, newName);
		newNode->type = newType;
		return 0;
	}
	else{
		struct Node* newNode = RequireNode();
		if(!newNode) return -2;
		newNode->Brother = 0;
		newNode->Parent = currentNode;
		newNode->Firstchild = 0;
		newNode->Lastchild = 0;
		strcpy(newNode->Name, newName);
		newNode->type = newType;
		currentNode->Lastchild->Brother = newNode;
		currentNode->Lastchild = newNode;
		return 0;
	}
}

int Delete_Node(struct Node* content, int n)
{
	if(n < 1) return -1;
	struct Node* ptr = content;
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

void Rename_Node(struct Node* content, char* newname)
{
	strcpy(content->Name,newname);
}

struct Node* GetNode(struct Node* content, int n)
{
	if(n < 1) return 0;
	struct Node* ptr = content;
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
	else if(content->Firstchild == 0){
		struct Node* newNode = RequireNode();
		if(!newNode) return -2;
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
////////////////////////////////////////////Function of Window//////////////////////////////////////////////

void ReleaseWindow(struct Window* aim)
{
	aim->Cur_icon = -1;
	aim->Cur_Node = 0;
	aim->next =0;
	aim->pre = 0;
	aim->Pos_x = 0;
	aim->Pos_y = 0;
	aim->ChoosenOne = 0;
	aim->lay1 = 0;
	aim->lay2 = 0;
	NumOfWindow--;
}

struct Window* RequireWindow()
{
	int i;
	if(NumOfWindow >= MaxWindow)
		return 0;
	for(i = 0; i <MaxWindow; i++)
	{
		if(window[i].Cur_icon == -1)
		{
			NumOfWindow++;
			return &window[i];
		}
	}
	return 0;
}

void ls_Window()
{
	struct Window* ptr = WindowLine->next;
	while(ptr != 0)
	{
		cprintf("%s\n",ptr->Cur_Node->Name);
		ptr = ptr->next;
	}
}

void InitWindow()
{
	NumOfWindow = 0;

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
	WindowLine->lay1 = 0;
	WindowLine->lay2 = 0;
}

struct Window* Add_Window(int icon)
{
	struct Window* newWindow = RequireWindow();
	if(!newWindow) return 0;

	newWindow->Cur_icon = icon;
	newWindow->Cur_Node = Root;
	newWindow->ChoosenOne = 0;
	newWindow->lay1 = 0;
	newWindow->lay2 = 0;

	if(WindowLine->next == 0)
	{
		WindowLine->next = newWindow;
		newWindow->next = 0;
		newWindow->pre = 0;
		newWindow->Pos_x = 20;
		newWindow->Pos_y = 20;
	}else
	{
		struct Window* ptr = WindowLine->next;
		newWindow->next = ptr;
		ptr->pre = newWindow;
		newWindow->pre = 0;
		WindowLine->next = newWindow;
		if(ptr->Pos_x == 100)
		{
			newWindow->Pos_x = 20;
			newWindow->Pos_y = 20;
		}
		else
		{
			newWindow->Pos_x = ptr->Pos_x+20;
			newWindow->Pos_y = ptr->Pos_y+20;
		}
	}
	return newWindow;
}

void Close_Window()
{
	if(WindowLine->next == 0) return;
	struct Window* ptr = WindowLine->next;
	struct Window* q = ptr->next;
	if(q == 0)
	{
		WindowLine->next = 0;
		ReleaseWindow(ptr);
	}else
	{
		WindowLine->next = q;
		q->pre = 0;
		ReleaseWindow(ptr);
	}
}

void Focus(struct Window* aim)
{
	if(WindowLine->next == aim) return;
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
	struct Window* ptr = WindowLine->next;
	struct Window* qtr = WindowLine->next;
	while(ptr != 0)
	{
		qtr = ptr;
		ptr = ptr->next;
	}
	return qtr;
}

struct Window* Click_Get_Window(int px, int py)
{
	struct Window* ptr = WindowLine->next;
	int posx, posy;
	while(ptr != 0)
	{
		posx = ptr->Pos_x;
		posy = ptr->Pos_y;
		if(px <= posx+WindowWidth && px >= posx && py <= posy+WindowHeight && py >= posy)
			break;
		ptr = ptr->next;
	}
	return ptr;
}

struct Window* get_window_by_icon(int icon)
{
	struct Window* win = WindowLine->next;
	while(win != 0)
	{
		if(win->Cur_icon == icon)
			return win;
		win = win->next;
	}
	return 0;
};

void Set_Window_Node(struct Window* wd, struct Node* node)
{
	if(wd != 0 && !node->type)
		wd->Cur_Node = node;
}

void Set_Window_Icon(struct Window* wd, int type)
{
	if(type >= 0 && wd != 0)
		wd->Cur_icon = type;
}

/*
void main()
{
	InitNode();
	InitWindow();
	
	Display(Root);
	Nodels(Root);

	Add_Node(User_folder,"tttt.txt",1);
	Add_Node(User_folder,"ttt.txt",1);
	Remove_Node(Root,User_folder);
	Display(Root);
	Nodels(Root);
	Display(GetNode(Root,3));
	Nodels(GetNode(Root,3));

	//Delete_Node(User_folder,1);
	//ls(User_folder);
	//ls(GetNode(Root,3));
	
	//Delete_Node(Root,1);
	//Delete_Node(Root,1);
	//Rename_Node(User_folder,"sb");
	//Display(Root);
	//ls(Root);
	
	Add_Window();
	Add_Window();
	Add_Window();
	ls_Window();
	cprintf("b1!\n");
	Close_Window();
	ls_Window();
	cprintf("b2!\n");
	Set_Window_Node(Click_Get_Window(60,60),User_folder);
	ls_Window();
	cprintf("b3!\n");
	Focus(Get_LastWindow());
	ls_Window();
	cprintf("b4!\n");		
}
*/