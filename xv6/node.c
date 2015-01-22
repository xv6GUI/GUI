/**************************************************************************
**  node.h
**  Used for file list
**  author: Wang Chengpeng
**  time:   2014.12.11
**  revised: Yang Yanzhe, time: 2015.1.22
***************************************************************************/

#include "types.h"
#include "defs.h"
#include "node.h"

int NumOfNode;                                //已申请的结点数
//struct Node* Root;                            //根结点
struct Node node[MaxNode];                    //模拟的结点内存块

void InitNode()
{
	NumOfNode = 0;

	//初始化结点类型
	int i;
	for(i = 0; i < MaxNode; i++)
	{
		node[i].NodeType = -1;
		node[i].Parent = 0;
		node[i].Firstchild = 0;
		node[i].Lastchild = 0;
		node[i].Brother = 0;
	}

	/*
	Root = RequireNode();
	
	//初始化根结点,添加一个文件和文件夹
	strcpy(Root->Name,"Root");
	Root->NodeType = 0;
	Root->Parent = 0;
	Root->Firstchild = 0;
	Root->Lastchild = 0;
	Root->Brother = 0;
	*/
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
	if(n < 1) return 0;

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


char* strcpy(char *s, char *t)
{
    char *temp;
    temp = s;

    while((*s++ = *t++) != 0);

    return temp;
}
