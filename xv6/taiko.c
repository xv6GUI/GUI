#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
#include "gui.h"
#include "taiko.h"
int game_x = 2, game_y = 3;

int checkGame(){
	int i, j;
	int okNum = 0;
	for (i = 0; i < 3; i++){
		for (j = 0; j < 4; j++){
			if (taiko[i][j] == i * 4 + j)
				okNum ++;
		}
	}
	if (okNum == 11 && taiko[2][3] == 12){
		taiko[2][3] = 11;
		return 1;
	}
	return 0;
}

int taikoGame(int flag, int x, int y){
	int i = game_x, j = game_y;
	int temp;
	if (flag == KBD_RIGHT){
		if (game_y - 1 >= 0){
			temp = taiko[i][j];
			taiko[i][j] = taiko[i][j - 1];
			taiko[i][j - 1] = temp;
			j = j - 1;
		}
	}
	if (flag == KBD_LEFT){
		if (game_y + 1 <= 3){
			temp = taiko[i][j];
			taiko[i][j] = taiko[i][j+1];
			taiko[i][j+1] = temp;
			j = j + 1;
		}
	}
	if (flag == KBD_UP){
		if (game_x + 1 <= 2){
			temp = taiko[i][j];
			taiko[i][j] = taiko[i+1][j];
			taiko[i+1][j] = temp;
			i = i + 1;
		}
	}
	if (flag == KBD_DOWN){
		if (game_x - 1 >= 0){
			temp = taiko[i][j];
			taiko[i][j] = taiko[i-1][j];
			taiko[i-1][j] = temp;
			i = i - 1;
		}
	}
	game_x = i;
	game_y = j;
	int isVistory = checkGame();
	drawTaikoFull(taiko, x, y);
	renderScreen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	return isVistory;
}
