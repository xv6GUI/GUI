/**************************************************************************
**  folder.c
**  render the file in folders
**  author: yangyanzhe
**  time:   2015.01.21
***************************************************************************/

#include "node.h"
#include "types.h"
#include "defs.h"
#include "gui.h"
#include "folder.h"

enum FileEvent file;
struct Node* currentFolder;

uint folderX, folderY;  //record the folder window start x, y

uint fileXMAX = 0;
uint fileYMAX = 0;
uint fileXMIN = FILE_DIV_X1;
uint fileYMIN = FILE_DIV_Y1;

uint chooseId = 0;
uint hasChoosed = 0;
struct Node* copyFileNode = 0;
struct Node* clickFile = 0;

uint
isInFolder(uint x, uint y)
{
	if(x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT)	return 1;
	else	return 0;
}

void
folderinit(uint win_x, uint win_y, struct Node* folder)
{
	//cprintf(folder->Name);
	int x, y;
	uint fileNum = 1;
	folderX = win_x;
	folderY = win_y;
	currentFolder = folder;
	x = 0; y=0;
	
	drawWindow(0, win_x, win_y);
		
	while(1)
	{	
		struct Node* child = GetNode(folder, fileNum);
		
		if(child == 0) break;
		x = win_x+FILE_DIV_X1 + FILE_DIV_WIDTH * ((fileNum-1) % 7);
		y = win_y + FILE_DIV_Y1 + FILE_DIV_HEIGHT * ((fileNum-1) / 7);
		if(x > fileXMAX)	fileXMAX = x;
		if(y > fileYMAX)	fileYMAX = y;

		drawFileIcon(child->NodeType, x, y);
		fileNum++;
	}

	renderScreen(win_x, win_y, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void
createFile()
{
	struct Node* temp;
	temp = RequireNode();
	strcpy(temp->Name,"blank.txt");
	temp->NodeType = 1;
	temp->Parent = currentFolder;
	temp->Brother = 0;

	currentFolder -> Lastchild -> Brother = temp;
	currentFolder -> Lastchild = temp;
}

void
createFolder()
{
	struct Node* temp;
	temp = RequireNode();
	strcpy(temp->Name,"blank");
	temp->NodeType = 0;
	temp->Parent = currentFolder;
	temp->Brother = 0;

	currentFolder -> Lastchild -> Brother = temp;
	currentFolder -> Lastchild = temp;
}

void
copyFile()
{
	if(!hasChoosed)	return;
	copyFileNode = clickFile;
	cancelClick();
}

void
deleteFile()
{
	if(!hasChoosed)	return;
	Delete_Node(currentFolder, chooseId);
	cancelClick();
}

void
pasteFile()
{
	if(!copyFileNode)	return;

	struct Node* temp;

	temp = RequireNode();
	strcpy(temp->Name,copyFileNode->Name);
	temp->NodeType = copyFileNode->NodeType;
	temp->Parent = currentFolder;
	temp->Brother = 0;

	currentFolder -> Lastchild -> Brother = temp;
	currentFolder -> Lastchild = temp;
}

void
folderkey(int key)
{
}

void
folderclick(uint posX, uint posY, struct Node* folder)
{
	int x, y;
	int line, row;

	posX -= folderX;
	posY -= folderY;
	
	// posX and posY are relative value now

	if(isInFolder(posX, posY))
	{
		// menu
		if(posY >= MENU_Y1 && posY <= MENU_Y2)
		{
			if(NEW_FOLDER_X1 <= posX && posX <= NEW_FOLDER_X2)	createFolder();
			else if(NEW_FILE_X1 <= posX && posX <= NEW_FILE_X2) 	createFile();
			else if(DELETE_X1 <= posX && posX <= DELETE_X2)		deleteFile();
			else return;
			
			folderinit(folderX, folderY, currentFolder);
			renderScreen(folderX, folderY, WINDOW_WIDTH, WINDOW_HEIGHT);
		}
		
		//icon
		if(posX <= fileXMAX && posX >= fileXMIN && posY <= fileYMAX && posY >= fileYMIN)
		{
			if(hasChoosed){
				folderinit(folderX, folderY, folder);	
				row = (chooseId-1) / 7;				//第几行
				line = (chooseId-1) % 7;			//第几列
				x = FILE_DIV_X1 + line * FILE_DIV_WIDTH + folderX - 11;
				y = FILE_DIV_Y1 + row  * FILE_DIV_HEIGHT + folderY - 11;
				renderScreen(x, y, FILE_ICON2_SIZE, FILE_ICON2_SIZE);
				hasChoosed = 0;
			}

			row = (posY - fileYMIN) / FILE_DIV_HEIGHT;	//第几行
			line = (posX - fileXMIN) / FILE_DIV_WIDTH;	//第几列
			chooseId = row * 7 + line + 1;

			clickFile = GetNode(folder, chooseId);
			
			if(clickFile == 0)   return;
			x = FILE_DIV_X1 + line * FILE_DIV_WIDTH + folderX - 11;
			y = FILE_DIV_Y1 + row  * FILE_DIV_HEIGHT + folderY - 11;
			
			drawChoosedFileIcon(clickFile->NodeType, x, y);
			renderScreen(x, y, FILE_ICON2_SIZE, FILE_ICON2_SIZE);
			hasChoosed = 1;

		}
		else{
			cancelClick();
		}
	}
}

void
cancelClick()
{
	int row, line;
	uint x, y;

	if(hasChoosed){
		folderinit(folderX, folderY, currentFolder);	
		row = (chooseId-1) / 7;				//第几行
		line = (chooseId-1) % 7;			//第几列
		x = FILE_DIV_X1 + line * FILE_DIV_WIDTH + folderX - 11;
		y = FILE_DIV_Y1 + row  * FILE_DIV_HEIGHT + folderY - 11;
		renderScreen(x, y, FILE_ICON2_SIZE, FILE_ICON2_SIZE);
		hasChoosed = 0;
		clickFile = 0;
	}
}

void 
initFileList()
{
	struct Node* HW1, *HW2, *HW3;
	struct Node* Cat;
	struct Node* Dog;
	struct Node* Movie;
	struct Node* Snake;
		
	Computer = RequireNode();
	strcpy(Computer->Name,"Computer");
	Computer->NodeType = 0;
	Computer->Parent = 0;
	Computer->Brother = 0;

	Snake = RequireNode();
	strcpy(Snake->Name,"Snake.exe");
	Snake->NodeType = 4;
	Snake->Parent = Computer;
	Snake->Firstchild = 0;
	Snake->Lastchild = 0;
	Snake->Brother = 0; 

	Movie = RequireNode();
	strcpy(Movie->Name,"Movie.flv");
	Movie->NodeType = 3;
	Movie->Parent = Computer;
	Movie->Firstchild = 0;
	Movie->Lastchild = 0;
	Movie->Brother = Snake; 

	Dog = RequireNode();
	strcpy(Dog->Name,"Dog.jpg");
	Dog->NodeType = 2;
	Dog->Parent = Computer;
	Dog->Firstchild = 0;
	Dog->Lastchild = 0;
	Dog->Brother = Movie; 

	Cat = RequireNode();
	strcpy(Cat->Name,"Cat.png");
	Cat->NodeType = 2;
	Cat->Parent = Computer;
	Cat->Firstchild = 0;
	Cat->Lastchild = 0;
	Cat->Brother = Dog; 
	
	Homework = RequireNode();
	strcpy(Homework->Name,"Homework");
	Homework->NodeType = 0;
	Homework->Parent = Computer;
	Homework->Brother = Cat;

	HW3 = RequireNode();
	strcpy(HW3->Name,"HW3.txt");
	HW3->NodeType = 1;
	HW3->Parent = Homework;
	HW3->Firstchild = 0;
	HW3->Lastchild = 0;
	HW3->Brother = 0; 

	HW2 = RequireNode();
	strcpy(HW2->Name,"HW2.txt");
	HW2->NodeType = 1;
	HW2->Parent = Homework;
	HW2->Firstchild = 0;
	HW2->Lastchild = 0;
	HW2->Brother = HW3; 

	HW1 = RequireNode();
	strcpy(HW1->Name,"HW1.txt");
	HW1->NodeType = 1;
	HW1->Parent = Homework;
	HW1->Firstchild = 0;
	HW1->Lastchild = 0;
	HW1->Brother = HW2; 	

	Homework->Firstchild = HW1;
	Homework->Lastchild = HW3;

	Computer->Firstchild = Homework;
	Computer->Lastchild = Snake;
}



