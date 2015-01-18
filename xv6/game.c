#include "types.h"
#include "defs.h"
#include "GAME.h"
#include "window.h"
#include "gui.h"

#define WindowWidth 660
#define WindowHeight 400
#define MineNum 100
#define PlaceWidth 25
#define PlaceHeight 15

#define winCount 275
/*
	record the state
	0:non-clicked 
	1:clicked
	2:flaged
*/
unsigned short state[PlaceWidth][PlaceHeight];
unsigned int game_x_pos, game_y_pos;
/*
	record the user state
	0:playing
	1:win
	2:fail
*/
unsigned short userState;
unsigned short openCount;
int start_flag = 0;

void handleLeftClickEvent(unsigned int x, unsigned int y);

void
game_end()
{
	cprintf("lose a game\n");
	int i, j;
	userState = 2;
	for(i = 1; i < WindowWidth - 1; i++)
		for(j = 20; j < WindowHeight - 20; j++)
			draw_point(i + game_x_pos, j + game_y_pos, failure[i][j]);
	for(i = 0; i < 48; i++)
    	for(j = 0; j < 48; j++)
    		if(refresh_game[i][j] != 0)
    			draw_point(game_x_pos + 595 + i, game_y_pos + 75 + j, refresh_game[i][j]);
	display_to_screen(game_x_pos, game_y_pos, WindowWidth, WindowHeight);
}

void
game_win()
{
	userState = 1;
}

int
is_correct(unsigned int x, unsigned int y)
{
	int count = 0;
	if(x > 0)
		if(state[x-1][y] == 2)
			count += 1;
	if(x < PlaceWidth - 1)
		if(state[x+1][y] == 2)
			count += 1;
	if(y > 0)
		if(state[x][y-1] == 2)
			count += 1;
	if(y < PlaceHeight - 1)
		if(state[x][y+1] == 2)
			count += 1;
	if(x > 0 && y > 0)
		if(state[x-1][y-1] == 2)
			count += 1;
	if(x > 0 && y < PlaceHeight - 1)
		if(state[x-1][y+1] == 2)
			count += 1;
	if(x < PlaceWidth - 1 && y > 0)
		if(state[x+1][y-1] == 2)
			count += 1;
	if(x < PlaceWidth - 1 && y < PlaceHeight - 1)
		if(state[x+1][y+1] == 2)
			count += 1;

	if(count < mines[x][y])
		return 2;

	if(x > 0)
		if((state[x-1][y] == 2 && mines[x-1][y] != -1) ||
			(state[x-1][y] != 2 && mines[x-1][y] == -1))
			return 0;
	if(x < PlaceWidth - 1)
		if((state[x+1][y] == 2 && mines[x+1][y] != -1) ||
			(state[x+1][y] != 2 && mines[x+1][y] == -1))
			return 0;
	if(y > 0)
		if((state[x][y-1] == 2 && mines[x][y-1] != -1) ||
			(state[x][y-1] != 2 && mines[x][y-1] == -1))
			return 0;
	if(y < PlaceHeight - 1)
		if((state[x][y+1] == 2 && mines[x][y+1] != -1) ||
			(state[x][y+1] != 2 && mines[x][y+1] == -1))
			return 0;
	if(x > 0 && y > 0)
		if((state[x-1][y-1] == 2 && mines[x-1][y-1] != -1) ||
			(state[x-1][y-1] != 2 && mines[x-1][y-1] == -1))
			return 0;
	if(x > 0 && y < PlaceHeight - 1)
		if((state[x-1][y+1] == 2 && mines[x-1][y+1] != -1) ||
			(state[x-1][y+1] != 2 && mines[x-1][y+1] == -1))
			return 0;
	if(x < PlaceWidth - 1 && y > 0)
		if((state[x+1][y-1] == 2 && mines[x+1][y-1] != -1) ||
			(state[x+1][y-1] != 2 && mines[x+1][y-1] == -1))
			return 0;
	if(x < PlaceWidth - 1 && y < PlaceHeight - 1)
		if((state[x+1][y+1] == 2 && mines[x+1][y+1] != -1) ||
			(state[x+1][y+1] != 2 && mines[x+1][y+1] == -1))
			return 0;

	return 1;
}

void
open_more(unsigned int x, unsigned int y)
{
	if(x > 0)
		if(state[x-1][y] == 0)
			handleLeftClickEvent(x - 1, y);
	if(x < PlaceWidth - 1)
		if(state[x+1][y] == 0)
			handleLeftClickEvent(x + 1, y);
	if(y > 0)
		if(state[x][y-1] == 0)
			handleLeftClickEvent(x, y - 1);
	if(y < PlaceHeight - 1)
		if(state[x][y+1] == 0)
			handleLeftClickEvent(x, y + 1);
	if(x > 0 && y > 0)
		if(state[x-1][y-1] == 0)
			handleLeftClickEvent(x - 1, y - 1);
	if(x > 0 && y < PlaceHeight - 1)
		if(state[x-1][y+1] == 0)
			handleLeftClickEvent(x - 1, y + 1);
	if(x < PlaceWidth - 1 && y > 0)
		if(state[x+1][y-1] == 0)
			handleLeftClickEvent(x + 1, y - 1);
	if(x < PlaceWidth - 1 && y < PlaceHeight - 1)
		if(state[x+1][y+1] == 0)
			handleLeftClickEvent(x + 1, y + 1);
}

void
handleLeftClickEvent(unsigned int x, unsigned int y)
{
	//ever clicked
	if(state[x][y] != 0)
		return;
	//bomb
	if(mines[x][y] == -1)
		game_end();
	//safely clicked
	else
	{
		openCount += 1;
		state[x][y] = 1;
		int i, j;
		for(i = 1; i < 19; i++)
			for(j = 1; j < 19; j++)
				//if(numberS[mines[x][y]][i][j] != 65535)
				draw_point(game_x_pos + 80 + 20 * x + i, game_y_pos + 50 + 20 * y + j, numberS[mines[x][y]][i][j]);
	}
	display_to_screen(game_x_pos + 80 + 20 * x, game_y_pos + 50 + 20 * y, 20, 20);
	if(mines[x][y] == 0)
		open_more(x, y);
}

void
handleRightClickEvent(unsigned int x, unsigned int y)
{
	if(state[x][y] == 2)
	{
		state[x][y] = 0;
		int i, j;
		for(i = 0; i < 20; i++)
			for(j = 0; j < 20; j++)
				draw_point(game_x_pos + 80 + 20 * x + i, game_y_pos + 50 + 20 * y + j, box[i][j]);
	}
	else if(state[x][y] == 0)
	{
		state[x][y] = 2;
		int i, j;
		for(i = 0; i < 20; i++)
			for(j = 0; j < 20; j++)
				if(flag[i][j] != 65535)
					draw_point(game_x_pos + 80 + 20 * x + i, game_y_pos + 50 + 20 * y + j, flag[i][j]);
	}
	else
		return;
	display_to_screen(game_x_pos + 80 + 20 * x, game_y_pos + 50 + 20 * y, 20, 20);
}

void
handleDoubleClickEvent(unsigned int x, unsigned int y)
{
	if(state[x][y] != 1)
		return;
	//safely clicked
	else
	{
		int flag = is_correct(x, y);
		if(flag == 1)
			open_more(x, y);
		else if(flag == 2)
			return;
		else
			game_end();
	}
}

void
pointInGameWindow(unsigned int x, unsigned int y, int flag)
{
	if(x > 595 && x < 643 && y > 75 && y < 123 && flag == LEFT_CLICK)
	{
		start_flag = 0;
		initGameWindow(game_x_pos, game_y_pos);
		display_to_screen(game_x_pos, game_y_pos, WindowWidth, WindowHeight);
	}
	else if(x < 80 || x > 580 || y < 50 || y > 350)
		return;
	else if(userState == 1 || userState == 2)
		return;
	else
	{
		int click_x_pos = (x - 79) / 20;
		int click_y_pos = (y - 49) / 20;
		if(flag == LEFT_CLICK)
			handleLeftClickEvent(click_x_pos, click_y_pos);
		else if(flag == RIGHT_CLICK)
			handleRightClickEvent(click_x_pos, click_y_pos);
		else if(flag == DOUBLE_CLICK)
			handleDoubleClickEvent(click_x_pos, click_y_pos);
	}
	if(openCount == winCount)
		game_win();
}

void 
initGameWindow(unsigned int x, unsigned int y)
{
	game_x_pos = x;
	game_y_pos = y;
	//GUI part
	int i, j, k, l;
	if(start_flag == 0)
	{
		start_flag = 1;
		for(i = 0; i < PlaceWidth; i++)
		{
			for(j = 0; j < PlaceHeight; j++)
			{
				state[i][j] = 0;
			}
		}
		userState = 0;
		openCount = 0;
	}
	for(i = 1; i < WindowWidth - 1; i++)
    {
		for(j = 20; j < WindowHeight - 20; j++)
		{
			draw_point(i + x, j + y, GAME_BACKGROUND[i][j]);
		}
    }

    //box part
    if(userState != 2)
    {
	    for(i = 0; i < PlaceWidth; i++)
	    {
	    	for(j = 0; j < PlaceHeight; j++)
	    	{
	    		//non-clicked yet
	    		if(state[i][j] == 0)
	    		{
		    		for(k = 0; k < 20; k++)
		    			for(l = 0; l < 20; l++)
		    				draw_point(x + 80 + 20 * i + k, y + 50 + 20 * j + l, box[k][l]);
	    		}
	    		else if(state[i][j] == 1)
	    		{
	    			for(k = 0; k < 20; k++)
		    			for(l = 0; l < 20; l++)
		    			{
		    				if(k == 0 || k == 19 || l == 0 || l == 19)
		    					draw_point(x + 80 + 20 * i + k, y + 50 + 20 * j + l, box[k][l]);
		    				else
		    					draw_point(x + 80 + 20 * i + k, y + 50 + 20 * j + l, numberS[mines[i][j]][k][l]);
		    			}
	    		} 
	    		else if(state[i][j] == 2)
	    		{
	    			for(k = 0; k < 20; k++)
		    			for(l = 0; l < 20; l++)
		    			{
		    				if(k == 0 || k == 19 || l == 0 || l == 19)
		    					draw_point(x + 80 + 20 * i + k, y + 50 + 20 * j + l, box[k][l]);
		    				else
		    					draw_point(x + 80 + 20 * i + k, y + 50 + 20 * j + l, flag[k][l]);
		    			}
	    		}
	    	}	
	    }
    }
    else
    {
    	for(i = 1; i < WindowWidth - 1; i++)
			for(j = 20; j < WindowHeight - 20; j++)
				draw_point(i + game_x_pos, j + game_y_pos, failure[i][j]);
		for(i = 0; i < 48; i++)
	    	for(j = 0; j < 48; j++)
	    		if(refresh_game[i][j] != 0)
	    			draw_point(game_x_pos + 595 + i, game_y_pos + 75 + j, refresh_game[i][j]);
    }
    for(k = 0; k < 48; k++)
    	for(l = 0; l < 48; l++)
    		if(refresh_game[k][l] != 0)
    			draw_point(x + 595 + k, y + 75 + l, refresh_game[k][l]);

    //display_to_screen(x, y, WindowWidth, WindowHeight);
    
    /*
    handleLeftClickEvent(0, 0);
    handleLeftClickEvent(0, 4);
    handleLeftClickEvent(2, 4);
    handleLeftClickEvent(0, 5);
    handleLeftClickEvent(1, 5);
    handleRightClickEvent(10, 10);
    */
}