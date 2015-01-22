/**************************************************************************
**  node.h
**  Used for file list
**  author: Wang Chengpeng
**  time:   2014.12.11
**  revised: Yang Yanzhe, time: 2015.1.22
***************************************************************************/

#define MaxNode 100

typedef struct Node
{
	int NodeType;				//-1:null, 0: folder, 1: txt, 2: png, 3: video, 4: exe
	struct Node* Parent;			
	struct Node* Firstchild;                
	struct Node* Lastchild;              
	struct Node* Brother;                   
	char Name[30];                        
} Node;

char* strcpy(char *s, char *t);                 

void InitNode();
void ReleaseNode(struct Node* p);						
void RenameNode(struct Node* p, char* newname);

int Add_Node(struct Node* currentNode, char* newName, int newType);	//添加指定名称和类型的结点到当前结点处
int Delete_Node(struct Node* p, int n);					//删除结点p的第n个孩子
int Remove_Node(struct Node* content, struct Node* old);                //复制old到content目录下，其中content为folder


struct Node* GetNode(struct Node* p, int n);    //获得结点p的第n个孩子
struct Node* RequireNode();			//申请内存空间

