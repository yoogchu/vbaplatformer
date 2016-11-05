#include <stdlib.h>
#include <stdio.h>
#include "mylib.h"
#include "text.h"
#include "./images/goku_main.h"
#include "./images/goku_dead.h"
#include "./images/goku_stand.h"
#include "./images/goku_standL.h"
#include "./images/goku_run1.h"
#include "./images/goku_run1L.h"
#include "./images/goku_jump1.h"
#include "./images/goku_jump1L.h"
#include "./images/goku_dash4.h"
#include "./images/goku_dash4L.h"
#include "./images/platform_up.h"
#include "./images/platform_left.h"
#include "./images/platform_right.h"



#define PLAYER_SPEED 2
#define GOKU_STAND_HEIGHT 48
#define DASH_LENGTH 50
#define GOKU_DASH_HEIGHT 33

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
				state = GAME;
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
	setColor(BLACK);
	drawString(20, 90, "GET HIGH", GREEN);
	
    	drawImage3(160 - GOKU_MAIN_HEIGHT,0, GOKU_MAIN_WIDTH, GOKU_MAIN_HEIGHT, goku_main);    
	drawString(50, 45, "Goku wants to get high!", WHITE);
	drawString(75, 60, "Press Start to Play", WHITE);

	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}

int game() {
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	setColor(BLACK);

	PLAYER player = {160 - GOKU_STAND_HEIGHT + 5, 0, GOKU_STAND_WIDTH, GOKU_STAND_HEIGHT, 2, 1, RIGHT, STAND};
	PLAYER oldPlayer = player;

    	waitForVblank();
	drawPlayer(player);

	waitForVblank();
	PLATFORM platform = {rand()%120, rand()%(240 - PLATFORM_UP_WIDTH), PLATFORM_UP_WIDTH, PLATFORM_UP_HEIGHT, rand()%4, UP};
	drawPlatform(platform);    	
	int isValidJump = 0;
	int isValidDash = 0;
    
	while(1) {
		player.row += 2;
		if (player.row > 160 - GOKU_STAND_HEIGHT) player.row = 160 - GOKU_STAND_HEIGHT;
		if (player.row < 0) player.row = 0;
		if (player.col < 0) player.col = 0;
		if (player.col > 240 - GOKU_RUN1_HEIGHT) player.col = 240 - GOKU_RUN1_HEIGHT;

		if (!KEY_DOWN_NOW(BUTTONS)) {
			player.stance = STAND;
		}
		if (KEY_DOWN_NOW(BUTTON_START)) {
			return END;
		} if (KEY_DOWN_NOW(BUTTON_UP) && (isValidJump == 0) && (player.doubleJump >0)) {
			player.stance = JUMP;
			player.row -= GOKU_STAND_HEIGHT;
			player.doubleJump -=1;

		  if (player.row < 0) player.row = 0;
		} if (KEY_DOWN_NOW(BUTTON_DOWN)) {
			player.row += PLAYER_SPEED;
			if (player.row > 160 - GOKU_STAND_HEIGHT) player.row = 160 - GOKU_STAND_HEIGHT;
		} if(KEY_DOWN_NOW(BUTTON_LEFT)) {
			player.stance = RUN;
			player.facing = LEFT;
			player.col -= PLAYER_SPEED;
			if (player.col < 0) player.col = 0;
		} if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
			player.stance = RUN;
			player.facing = RIGHT;
			player.col += PLAYER_SPEED;
			if (player.col > 240 - GOKU_RUN1_HEIGHT) player.col = 240 - GOKU_RUN1_HEIGHT;
		} if(KEY_DOWN_NOW(BUTTON_A) && (isValidDash == 0)) {
			player.stance = DASH;
			if (player.facing == LEFT) {
				player.col -= DASH_LENGTH;
                player.row += (oldPlayer.height - GOKU_DASH_HEIGHT) + oldPlayer.row;
			} else if (player.facing == RIGHT) {
				player.col += DASH_LENGTH;
                player.row += (oldPlayer.height - GOKU_DASH_HEIGHT) + oldPlayer.row;
			}
			if (player.col < 0) player.col = 0;
			if (player.col > 240 - GOKU_RUN1_HEIGHT) player.col = 240 - GOKU_RUN1_HEIGHT;
		}
		
		isValidJump = KEY_DOWN_NOW(BUTTON_UP);
		isValidDash = KEY_DOWN_NOW(BUTTON_A);
		waitForVblank();
		drawRect(oldPlayer.row, oldPlayer.col, oldPlayer.width, oldPlayer.height, BLACK);

		//setColor(BLACK);
        	drawPlayer(player);
        	oldPlayer = player;
	}
	return 1;
}
void end(){
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	setColor(BLACK);
	drawString(20, 85, "Game Over", RED);
	drawString(30, 70, "Your Score: ", WHITE);
	drawString(30, 160, score_buffer, WHITE);
	drawImage3(70, 100, GOKU_DEAD_WIDTH, GOKU_DEAD_HEIGHT, goku_dead);
	drawString(132, 45, "Press Select to Play Again", WHITE);

	while(!KEY_DOWN_NOW(BUTTON_SELECT));
	while(KEY_DOWN_NOW(BUTTON_SELECT));
}

