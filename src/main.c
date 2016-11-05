#include <stdlib.h>
#include <stdio.h>
#include "mylib.h"
#include "text.h"
#include "./images/goku_main.h"
#include "./images/goku_dead.h"

#define PLAYER_SPEED 2
#define NUMBER_JUMPS 2

enum {START, GAME, END};

void start();
int game();
void end();
char score_buffer[8];

int main() {
	int state = START;
	while(1) {
		switch(state) {
			case START:
				sprintf(score_buffer, "%i", 0);
				start();
				state = END;
				break;

			case GAME:
				state = game();
				break;

			case END:
				end();
				state = START;
				break;

			default:
				break;
		}
	}
}

void start() {
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	setColour(BLACK);
	drawString(20, 90, "GET HIGH", WHITE);
	
    	drawImage3(160 - GOKU_MAIN_HEIGHT,0, GOKU_MAIN_WIDTH, GOKU_MAIN_HEIGHT, goku_main);    
	drawString(50, 45, "Goku wants to get high!", WHITE);
	drawString(75, 60, "Press Start to Play", WHITE);

	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}

int game(){
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	setColour(BLACK);
	drawRect(0, 0, 240 - INFO_GUTTER_WIDTH, 160, WHITE);
	drawString(0, 240 - INFO_GUTTER_WIDTH, "LIVES:", WHITE);
//	drawChar(10, 240 - INFO_GUTTER_WIDTH, *plives + NUMERAL_OFFSET, WHITE);

/*
	DOODLER doodler = {160 - DOODLER_SIZE, 240 - INFO_GUTTER_WIDTH - DOODLER_SIZE, LEFT};
	DOODLER oldDoodler = doodler;
	int doodlerSpeed = DOODLER_SPEED_DEFAULT;
	JETPACK sJetpack = {rand()%50, rand()%50};

	ENEMY enemies [NUMENEMIES];
	ENEMY oldEnemies [NUMENEMIES];
	ENEMY *cur;
	int d[] = {-1, 0, 1};
	int numd = sizeof(d) / sizeof(d[0]);

	for (int i = 0; i < NUMENEMIES; i++)
	{
		enemies[i].row = rand()%60 + rand()%60;
		enemies[i].col = rand()%100 + 60;
		enemies[i].drow = d[rand()%numd];
		enemies[i].dcol = d[rand()%numd];
		while (!enemies[i].drow && !enemies[i].dcol)
		{
			enemies[i].drow = d[rand()%numd];
			enemies[i].dcol = d[rand()%numd];
		}
		oldEnemies[i] = enemies[i];
	}
	
	
	drawDoodler(doodler);
	for (int i = 0; i < NUMENEMIES; i++)
	{
		drawEnemy(enemies[i]);
	}
	drawJetpack(sJetpack);

	drawChar(76, 117, '3', BLUE);
	for (int i = 0; i < 500; i++)
	{
		waitForVblank();
	}
	
	drawChar(76, 117, '3', WHITE);
	drawChar(76, 117, '2', BLUE);
	for (int i = 0; i < 500; i++)
	{
		waitForVblank();
	}
	drawChar(76, 117, '2', WHITE);
	drawChar(76, 117, '1', BLUE);
	for (int i = 0; i < 500; i++)
	{
		waitForVblank();
	}
	drawChar(76, 117, '1', WHITE);
	while(1)
	{
		if(KEY_DOWN_NOW(BUTTON_B))
		{
			while(KEY_DOWN_NOW(BUTTON_B))
			{
				drawChar(76, 117, 'P', RED);
			}
			drawChar(76, 117, 'P', WHITE);
		}
		
		if(KEY_DOWN_NOW(BUTTON_UP))
		{
			doodler.row -= doodlerSpeed;
			if (doodler.row < 0)
			{
				doodler.row = 0;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_DOWN))
		{
			doodle:wq
r.row += doodlerSpeed;
			if (doodler.row > 160 - DOODLER_SIZE)
			{
				doodler.row = 160 - DOODLER_SIZE;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_LEFT))
		{
			doodler.col -= doodlerSpeed;
			doodler.facing = LEFT;
			if (doodler.col < 0)
			{
				doodler.col = 0;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_RIGHT))
		{
			doodler.col += doodlerSpeed;
			doodler.facing = RIGHT;
			if (doodler.col > 240 - INFO_GUTTER_WIDTH - DOODLER_SIZE)
			{
				doodler.col = 240 - INFO_GUTTER_WIDTH - DOODLER_SIZE;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_SELECT))
		{
			*plives = LIVES_DEFAULT;
			return GAME;
		}
		for (int i = 0; i < NUMENEMIES; i++)
		{
			cur = enemies + i;
			cur->row = cur->row + cur->drow;
			cur->col = cur->col + cur->dcol;
			if(cur->row < 0)
			{
				cur->row = 0;
				cur->drow=-cur->drow;
			}
			if(cur->row > 160 - ENEMY_SIZE)
			{
				cur->row = 160 - ENEMY_SIZE;
				cur->drow=-cur->drow;
			}

			if(cur->col < 0)
			{
				cur->col = 0;
				cur->dcol = -cur->dcol;
			}
			if(cur->col > 240 - INFO_GUTTER_WIDTH - ENEMY_SIZE)
			{
				cur->col = 240 - INFO_GUTTER_WIDTH - ENEMY_SIZE;
				cur->dcol =-cur->dcol;
			}
		}

		for (int i = 0; i < NUMENEMIES; i++)
		{
			if(enemyCollision(doodler, enemies[i]))
			{
				if (*plives)
				{
					*plives -= 1;
					return GAME;
				}
				else
				{
					return LOSE;
				}
			}
		}

		if(jetpackCollision(doodler, sJetpack))
		{
			return WIN;
		}


		waitForVblank();


		drawRect(oldDoodler.row, oldDoodler.col, DOODLER_SIZE, DOODLER_SIZE, WHITE);
		drawDoodler(doodler);
		oldDoodler = doodler;
		for (int i = 0; i < NUMENEMIES; i++)
		{
			drawRect(oldEnemies[i].row, oldEnemies[i].col, ENEMY_SIZE, ENEMY_SIZE, WHITE);
		}
		for (int i = 0; i < NUMENEMIES; i++)
		{
			drawEnemy(enemies[i]);
			oldEnemies[i] = enemies[i];
		}

		drawJetpack(sJetpack);
*/
//	}
    return 1;
}

void end(){
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	setColour(BLACK);
	drawString(20, 85, "Game Over", RED);
	drawString(30, 85, "Your Score: ", WHITE);
	drawString(30, 160, score_buffer, WHITE);
	drawString(132, 45, "Press Start to Play Again", WHITE);

	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}

