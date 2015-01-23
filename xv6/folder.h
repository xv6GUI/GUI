/**************************************************************************
**  folder.h
**  render the file in folders
**  author: yangyanzhe
**  time:   2015.01.21
***************************************************************************/

#define MENU_Y1 32
#define MENU_Y2 80

#define NEW_FOLDER_X1 20
#define NEW_FOLDER_X2 100

#define NEW_FILE_X1 105
#define NEW_FILE_X2 175

#define DELETE_X1 180
#define DELETE_X2 250

#define RETURN_X1 255
#define RETURN_X2 340

#define COPY_X1 345
#define COPY_X2 430

#define PASTE_X1 435
#define PASTE_X2 515

#define FILE_DIV_X1 50
#define FILE_DIV_WIDTH 70
#define FILE_DIV_HEIGHT 100
#define FILE_DIV_Y1 100

#define FILE_LINE_WIDTH 

enum FileEvent{
    NONE = 0,
    NEW_FOLDER,
    NEW_FILE,
    DELETE,
    RENAME
};

struct Node* Computer;
struct Node* Homework;
struct Node* Trash;  

void folderinit(uint win_x, uint win_y, struct Node* folder);
void folderIconInit(uint win_x, uint win_y, struct Node* folder);
void folderclick(uint posX, uint posY, struct Node* folder);
void trashclick(uint posX, uint posY);


void initFileList();
void cancelClick();

void emptyTrash();
int  getTrashState();
void returnAllTrash();

void returnParent();
void folderkey(int key);
struct Node* getCurrentFolder();



