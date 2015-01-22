/**************************************************************************
**  window.h
**  multiple window list
**  Author: Wang Chengpeng, Yang Yanzhe
**  Revise Time: 2015.1.21
***************************************************************************/

#define MAX_WINDOW 20

typedef struct Window
{
	int type;		   	//-1: null, 1: WINDOW_COMPUTER,2: WINDOW_PAINT ...
	int x;			
	int y;		

	struct Window* next;		
	struct Window* pre;		
	struct Node* folder;  		//if it is a folder window, this will be used 
} Window;

          
void releaseWindow(struct Window* w);                     	//仅释放w指向的内存空间  
void closeWindow();                                                 	
             
void setWindowPosition(struct Window* aim, int offX, int offY);            
void setFocus(struct Window* aim);
void setWindowNode(struct Window* w, struct Node* node);
void setWindowType(struct Window* w, int type);   

struct Window* 	getLastWindow();				
struct Window* 	getWindowByPosition(int px, int py);		//点击（px,py)返回窗口
struct Window* 	getWindowByType(int type);          		//返回第一个窗口类型为type的窗口

struct Window* 	requireWindow();				//申请一块窗口内存空间
struct Window* 	addWindow(int icon);                              

struct Window	window[MAX_WINDOW];           	//模拟的窗口内存块
struct Window* 	WindowLine;                   	//窗口队列
int NumOfWindow;                              	//已申请的窗口数



