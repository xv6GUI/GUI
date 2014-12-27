#define WindowWidth 660
#define WindowHeight 400
#define MaxNode 50
#define MaxWindow 11

///////////////////////////////////////////////////////////////Node//////////////////////////////////////////////////////////////////////////////
typedef struct Node
{
	struct Node* Parent;
	struct Node* Firstchild;
	struct Node* Lastchild;
	struct Node* Brother;
	char Name[30];
	int type;//1: file; 0: folder;
} Node;

struct Node* RequireNode();
void ReleaseNode(struct Node*);
void InitNode();
int Add_Node(struct Node*,char*,int);
int Delete_Node(struct Node*, int);
void Rename_Node(struct Node*, char*);
struct Node* GetNode(struct Node*, int);
int Remove_Node(struct Node*, struct Node*);

//////////////////////////////////////////////////////////////Window//////////////////////////////////////////////////////////////////////////////
typedef struct Window
{
	int Pos_x;
	int Pos_y;
	int Cur_icon;
	int ChoosenOne;
	struct Node* Cur_Node;
	struct Window* next;
	struct Window* pre;
} Window;

void InitWindow();
void ReleaseWindow(struct Window*);
struct Window* RequireWindow();
struct Window* Add_Window(int);
void Close_Window();
void Focus(struct Window*);
struct Window* Get_LastWindow();
struct Window* Click_Get_Window(int, int);
struct Window* get_window_by_icon(int);
void Set_Window_Node(struct Window*, struct Node*);
void Set_Window_Icon(struct Window*, int);


////////////////////////////////////////////////////////////全局变量/////////////////////////////////////////////////////////////////////////////////
int NumOfNode;
int NumOfWindow;
struct Window window[MaxWindow];
struct Node node[MaxNode];
struct Window* WindowLine;
struct Node* Root;
struct Node* User_folder;
struct Node* Readme_file;



