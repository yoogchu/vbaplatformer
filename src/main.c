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




#define PLAYER_SPEED 2
#define DASH_LENGTH 40
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

//	PLAYER player = {160 - GOKU_STAND_HEIGHT + 5, 0, GOKU_STAND_WIDTH, GOKU_STAND_HEIGHT, 2, 1, RIGHT, STAND};
//	PLAYER oldPlayer = player;

    	
//	drawPlayer(player, 0);

//draw platforms
	int num_plat = (rand()%2)+2;
//	int num_plat = 1;
	PLATFORM platforms [num_plat];
 	
	PLAYER player = {0, 0, GOKU_STAND_WIDTH, GOKU_STAND_HEIGHT, 2, 1, RIGHT, STAND};

    	
    PLAYER  oldPlayer =	drawPlayer(player, 0);

	for(int i=0;i<num_plat;i++){
		if (i == 0) {
			platforms[i].row = 160-PLATFORM_UP_HEIGHT;
			platforms[i].col =  0;
			platforms[i].width = PLATFORM_UP_WIDTH;
			platforms[i].height = PLATFORM_UP_HEIGHT;
			platforms[i].facing = 0;
			drawPlatform(platforms[i]);
			continue;
		}
		platforms[i].facing = rand()%3;
	//	platforms[i].facing = 0;
		platforms[i].row = (rand()%120) - PLATFORM_LEFT_HEIGHT;
		platforms[i].col = rand()%(120 - PLATFORM_UP_WIDTH);

		if (platforms[i].facing == 2) {
			platforms[i].row-=60;
			platforms[i].height = PLATFORM_RIGHT_HEIGHT;
			platforms[i].width = PLATFORM_RIGHT_WIDTH;
		} else if (platforms[i].facing == 1) {
			platforms[i].col+=120;
			platforms[i].height = PLATFORM_LEFT_HEIGHT;
			platforms[i].width = PLATFORM_LEFT_WIDTH;
		} else if (platforms[i].facing == 0) {
			platforms[i].col+=60;
			platforms[i].height = PLATFORM_UP_HEIGHT;
			platforms[i].width = PLATFORM_UP_WIDTH;
		}
		if (platforms[i].row < 0) platforms[i].row = 0;
		if (platforms[i].row > 120) platforms[i].row = 120 - GOKU_STAND_HEIGHT - 20;
		
		drawPlatform(platforms[i]);
	}  	
	int isValidJump = 0;
	int isValidDash = 0;
	int frame = 0;
	int score = 0;
	while(1) {
		player.row += 2; 	//GRAVITY
		frame+=1;

        
       // 	if (player.row > 160 - player.height) player.row = 160-player.height;//return END;
	//	if (player.row < 0) player.row = 0;
	//	if (player.col < 0) player.col = 0;
	//	if (player.col > 240 - player.width) player.col = 240 - player.width;
		if (!KEY_DOWN_NOW(BUTTONS)) {
			player.stance = STAND;
			player.height = GOKU_STAND_HEIGHT;
		}
		
       		if (KEY_DOWN_NOW(BUTTON_START)) {
			return END;
		} 
        	if (KEY_DOWN_NOW(BUTTON_UP) && (isValidJump == 0) && (player.doubleJump>0) ) {
			player.stance = JUMP;
			player.row -= player.height/2;
			player.doubleJump--;
		} 
        
       		if (KEY_DOWN_NOW(BUTTON_DOWN)) {
			player.stance = DOWN;
			player.height = GOKU_DOWN_HEIGHT;
//			player.row += PLAYER_SPEED;
		} 
        
        	if(KEY_DOWN_NOW(BUTTON_LEFT)) {
			player.stance = RUN;
			player.facing = LEFT;
			player.col -= PLAYER_SPEED;
		} 
        
        	if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
			player.stance = RUN;
			player.facing = RIGHT;
			player.col += PLAYER_SPEED;
		} 
        
        	if (KEY_DOWN_NOW(BUTTON_A) && (isValidDash == 0) && (player.dash > 0)) {
			player.stance = DASH;
			if (player.facing == LEFT) {
				player.col -= DASH_LENGTH;
			} else if (player.facing == RIGHT) {
				player.col += DASH_LENGTH;
                		//player.row += (oldPlayer.height - GOKU_DASH_HEIGHT) + oldPlayer.row;
			}
		}
//collision check
		for (int i = 0;i < num_plat;i++) {
			if (player.row < platforms[i].row) {		//goku above platform
				if (checkCollision(player, platforms[i], 2)) {
					player.row = platforms[i].row - player.height;
					player.doubleJump = 2;
					player.dash = 1;
					score++;
					sprintf(score_buffer, "%i", score); 
				}
			} else if (player.row > platforms[i].row) {	//goku below platform
				if ((checkCollision(player, platforms[i], 1)) & (player.stance == JUMP)) {
					player.row = platforms[i].row + platforms[i].height;
					player.doubleJump = 2;
					player.dash = 1;
					score++;
					sprintf(score_buffer, "%i", score); 
				}
			}
		}
               	if (player.row > 160 - GOKU_STAND_HEIGHT) player.row = 160 - player.height;
		if (player.row < 0) player.row = 0;
		if (player.col < 0) player.col = 0;
		if (player.col + player.width > 240) player.col = 240 - GOKU_STAND_WIDTH - 5;


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
	drawString(132, 45, "Press Select to Play Again", WHITE);

	while(!KEY_DOWN_NOW(BUTTON_SELECT));
	while(KEY_DOWN_NOW(BUTTON_SELECT));
}

