#define WindowWidth 660
#define WindowHeight 400
#define MaxNode 60
#define MaxWindow 20

///////////////////////////////////////////////////////////////Node//////////////////////////////////////////////////////////////////////////////
typedef struct Node
{
	int type;																    //1表示文件，0表示文件夹
	struct Node* Parent;													    //父结点
	struct Node* Firstchild;                                                    //第一个孩子
	struct Node* Lastchild;                                                     //最后一个孩子
	struct Node* Brother;                                                       //兄弟结点
	char Name[30];                                                              //结点名称
} Node;

char* strcpy(char *s, char *t);                                                 //将t指向的字符串拷贝到s中，返回拷贝后的字符串指针
void InitNode();																//初始化结点空间
struct Node* RequireNode();														//申请内存空间
void ReleaseNode(struct Node* n);												//释放结点n指向的内存空间  
int Add_Node(struct Node* currentNode, char* newName, int newType);				//添加指定名称和类型的结点到当前结点处
int Delete_Node(struct Node* p, int n);											//删除结点p的第n个孩子
void Rename_Node(struct Node* p, char* newname);								//将结点p的名称更改为newname
struct Node* GetNode(struct Node* p, int n);                                    //获得结点p的第n个孩子
int Remove_Node(struct Node* content, struct Node* old);                        //复制old到content目录下，其中content为folder

//////////////////////////////////////////////////////////////Window//////////////////////////////////////////////////////////////////////////////
typedef struct Window
{
	int Cur_icon;																  //窗口类型
	int ChoosenOne;																  //内含结点中被选结点的下标
	struct Node* Cur_Node;														  //内含结点的根结点
	struct Window* next;														  //下一个窗口
	struct Window* pre;															  //上一个窗口
	int Pos_x;																	  //窗口左上角的横坐标
	int Pos_y;																	  //窗口左上角的纵坐标
} Window;

void InitWindow();                                                 //初始化窗口内存空间           
struct Window* RequireWindow();									   //申请一块窗口内存空间
void ReleaseWindow(struct Window* w);                              //释放w指向的内存空间  
struct Window* Add_Window(int icon);                               //增加窗口类型为icon的窗口
void Close_Window();                                               //关闭窗口
void Move_Window(struct Window* aim, int px, int py);              //移动aim窗口，距离分别为px.py
void Focus(struct Window* aim);                                    //把aim指向的窗口设置为最前    
struct Window* Get_LastWindow();								   //返回最后一个窗口
struct Window* Click_Get_Window(int px, int py);				   //点击（px,py)返回窗口
struct Window* get_window_by_icon(int type);                       //返回第一个窗口类型为type的窗口
void Set_Window_Node(struct Window* w, struct Node* node);        //将node设置为w的内含结点
void Set_Window_Icon(struct Window* w, int type);                 //设置w的窗口类型为type


////////////////////////////////////////////////////////////全局变量/////////////////////////////////////////////////////////////////////////////////
int NumOfNode;                                //已申请的结点数
int NumOfWindow;                              //已申请的窗口数
struct Window window[MaxWindow];              //模拟的窗口内存块
struct Node node[MaxNode];                    //模拟的结点内存块
struct Window* WindowLine;                    //窗口队列
struct Node* Root;                            //根结点
struct Node* User_folder;                     //用户文件夹
struct Node* Readme_file;                     //文件



