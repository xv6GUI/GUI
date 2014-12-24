////////////////////////////////////////////////////File////////////////////////////////////////////////////////////////////////
#define UNCERTAIN 0
#define FILE 1
#define FOLDER 2
#define MAXNODE 100

struct node
{
	char* name;                   //文件或者文件夹的名称
	int type;                     //FILE or FOLDER
	int childrenNum;              //如果节点为文件夹，记录文件夹中文件或者文件夹的个数

	node* Parent;                 //父节点，表示所在的文件夹
	node* FirstChild;             //包含的第一个文件或者文件夹
	node* Brother;                //兄弟节点，及下一个文件或者文件夹
};

node* root;	                      //全局指针，树状链表的头指针
node memory[MAXNODE];             //可供分配的内存，最多为MAXNODE
int AvailableMemory;              //剩余可供申请的内存数

void Initnode();
node* Newnode(int nodeType);
void Freenode(node* r);
int Addnode(node* parent, char* ch, int location, int nodeType);
int Deletenode(node* parent, int location);
void Renamenode(node* r, char* name);
node* Getnode(node* parent, int location);
int Removenode(node* parent, node* r);

////////////////////////////////////////////////window//////////////////////////////////////////////////////////////////////////////
#define MAXWINDOW 20
#define WINDOWWIDTH 600
#define WINDOWHEIGHT 400
#define ICON 0
#define LIST 1

struct window
{
	int wType;					         //窗口类型，图标类型或列表类型,初始时为-1
	int xPos;                            //窗口左上角的x坐标
	int yPos;                            //窗口左上角的y坐标

	node* content;                       //窗口内文件或文件夹链表的头指针,content->FirstChild为第一个内容
	int contentNum;                      //链表的长度，即窗口中文件或文件夹的总个数
	int focusIndex;                      //位于焦点的文件或文件夹的次序，从1开始计数

	window* pre;                         //窗口队列中的前一个窗口
	window* next;                        //窗口队列中的后一个窗口
};

window* windowHead;                      //窗口队列的头指针,windowHead->next为第一个窗口
window windowMemory[MAXWINDOW];          //可供分配的窗口类型变量的内存，最多可同时申请MAXWINDOW个
int AvailableWindowMemory;               //剩余可供申请的窗口变量个数

void InitWindow();
window* NewWindow(int type);
void FreeWindow(window* w);
int AddWindow(int type);
int CloseWindow();
window* GetWindow(int location);
window* get_window_by_icon(int icon);
int FocusWindow(window* w);
int MoveWindow(window* w, int dx, int dy);
window* ClickWindow(int x, int y);
int setWindowType(window* w, int type);
int setWindowContent(window* w, node* r);



