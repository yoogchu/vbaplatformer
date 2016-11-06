#include <stdlib.h>
#include <stdio.h>
#include "mylib.h"
#include "text.h"
#include "./images/goku_main.h"
#include "./images/goku_dead.h"
#include "./images/goku_stand.h"
#include "./images/platform_up.h"
#include "./images/platform_left.h"
#include "./images/platform_right.h"
#include "./images/goku_down.h"
#include "./images/goku_dash1.h"
#include "./images/goku_run1L.h"
#include "./images/goku_run1.h"


#define PLAYER_SPEED 2
#define DASH_LENGTH 40
#define GOKU_DASH_HEIGHT 33
#define JUMP_HEIGHT 35

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

	PLAYER player = {0,0, GOKU_STAND_WIDTH, GOKU_STAND_HEIGHT, 2, 1, RIGHT, STAND};
	PLAYER oldPlayer = drawPlayer(player, 0);

	int num_plat = (rand()%2)+2;
//	int num_plat = 2;
	int countL = 0;
	int countM = 0;
	int countR = 0;
	PLATFORM platforms [num_plat];
//draw platforms
	for(int i=0;i<num_plat;i++) {
		if (i == 0) {	//starting platform
			platforms[i].row = 160-PLATFORM_UP_HEIGHT;
			platforms[i].col =  0;
			platforms[i].width = PLATFORM_UP_WIDTH;
			platforms[i].height = PLATFORM_UP_HEIGHT;
			platforms[i].facing = 0;
			drawPlatform(platforms[i]);
			continue;
		}
		platforms[i].facing = rand()%3;
//		platforms[i].facing = 0;
		platforms[i].row = (rand()%120) + 30;
		platforms[i].col = rand()%(120 - PLATFORM_UP_WIDTH);

		if (platforms[i].facing == 2) {
			if (countR >= 2) {
				num_plat++;
				continue;
			}
			platforms[i].row-=40;
			platforms[i].height = PLATFORM_RIGHT_HEIGHT;
			platforms[i].width = PLATFORM_RIGHT_WIDTH;
			platforms[i].col = player.width + 5 + rand()%20;
			
			countR++;
		} else if (platforms[i].facing == 1) {
			if (countL >= 2) {
				num_plat++;
				continue;
			}
			platforms[i].col+=120;
			platforms[i].height = PLATFORM_LEFT_HEIGHT;
			platforms[i].width = PLATFORM_LEFT_WIDTH;
			
			countL++;
		} else if (platforms[i].facing == 0) {
			if (countM >= 2) {
				num_plat++;
				continue;
			}
			platforms[i].col+=60;
			platforms[i].height = PLATFORM_UP_HEIGHT;
			platforms[i].width = PLATFORM_UP_WIDTH;

			countM++;
		}
		if (platforms[i].row < 0) platforms[i].row = 0;
		if (platforms[i].row > 120) platforms[i].row = 120 - GOKU_STAND_HEIGHT - 20;
		
		drawPlatform(platforms[i]);
	}  	
	int isValidJump = 0;
	int isValidDash = 0;
	int frame = 0;
	int score = 0;
   	int hasLanded = 1;
	int currentJump = 0;
	int isJumping = 0;
//  	int hasDashed = 0;
	while(1) {
		
		if (player.row <= currentJump - JUMP_HEIGHT) isJumping = 0; 
		if (isJumping) {
			player.row -= 5;
			player.stance = JUMP;
			
		}
		player.row += 2; 	//GRAVITY
		frame+=1;		//animations

		if (hasLanded == 1) {
                	player.stance = STAND;
                	player.height = GOKU_STAND_HEIGHT;
            	} else {
			player.stance = oldPlayer.stance;
		}

//BUTTONS PRESSED
       		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			return END;
		}
        	if (KEY_DOWN_NOW(BUTTON_UP) && (isValidJump == 0) && (player.doubleJump>0) ) {
			currentJump = player.row;
			isJumping = 1;
			player.doubleJump--;
            		hasLanded = 0;
		} 
        
       		if (KEY_DOWN_NOW(BUTTON_DOWN)) {
			player.stance = DOWN;\
			player.width = GOKU_DOWN_WIDTH;
			player.height = GOKU_DOWN_HEIGHT;
//			player.row += PLAYER_SPEED;
		} 
        
        	if(KEY_DOWN_NOW(BUTTON_LEFT)) {
			player.stance = RUN;
			player.facing = LEFT;
			player.width = GOKU_RUN1L_WIDTH;
			player.height = GOKU_RUN1L_HEIGHT;
			player.col -= PLAYER_SPEED;
		} 
        
        	if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
			player.stance = RUN;
			player.facing = RIGHT;
			player.width = GOKU_RUN1_WIDTH;
			player.height = GOKU_RUN1_HEIGHT;
			player.col += PLAYER_SPEED;
		} 
        
        	if (KEY_DOWN_NOW(BUTTON_A) && (isValidDash == 0) && (player.dash > 0)) {
			player.stance = DASH;
			player.width = GOKU_DASH1_WIDTH;
			player.height = GOKU_DASH1_HEIGHT;
			if (player.facing == LEFT) {
				player.col -= DASH_LENGTH;
			} else if (player.facing == RIGHT) {
				player.col += DASH_LENGTH;
			}
			player.dash--;
		}
//collision check
		for (int i = 0;i < num_plat;i++) {
			if (player.row < platforms[i].row) {		//goku above platform
				if (checkCollision(player, platforms[i], 2)) {
					player.row = platforms[i].row - player.height;
					player.doubleJump = 2;
					player.dash = 1;
				//	score++;
					sprintf(score_buffer, "%i", score);
                    			hasLanded = 1;
				}
			} else if (player.row > platforms[i].row) {	//goku below platform
				if (checkCollision(player, platforms[i], 1) & isJumping) {
					player.row = platforms[i].row + platforms[i].height;
					isJumping = 0;
					player.row +=2;
					score++;
				}
			}
		}

//bounds
               	if (player.row > 160 - GOKU_STAND_HEIGHT) player.row = 160 - player.height;
		if (player.row < 0) {
			player.row = 0;
			isJumping = 0;
		}
		if (player.col < 0) player.col = 0;
		if (player.col + player.width > 240) player.col = 240 - player.width;


		isValidJump = KEY_DOWN_NOW(BUTTON_UP);
		isValidDash = KEY_DOWN_NOW(BUTTON_A);
        	drawRect(oldPlayer.row, oldPlayer.col, oldPlayer.width, oldPlayer.height, BLACK);

        	oldPlayer = drawPlayer(player, frame);
		waitForVblank();
		if (frame == 2) frame = -1;
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
	drawString(132, 45, "Press Start to Play Again", WHITE);

	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}

