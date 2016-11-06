#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "./images/goku_stand.h"
#include "./images/goku_standL.h"
#include "./images/goku_run1.h"
#include "./images/goku_run2.h"
#include "./images/goku_run3.h"
#include "./images/goku_run1L.h"
#include "./images/goku_run2L.h"
#include "./images/goku_run3L.h"
#include "./images/goku_jump.h"
#include "./images/goku_jumpL.h"
#include "./images/goku_down.h"
#include "./images/goku_downL.h"
#include "./images/goku_dash1.h"
#include "./images/goku_dash1L.h"
#include "./images/goku_dash2.h"
#include "./images/goku_dash2L.h"
#include "./images/goku_dash3.h"
#include "./images/goku_dash3L.h"
#include "./images/platform_up.h"
#include "./images/platform_left.h"
#include "./images/platform_right.h"
 
u16* videoBuffer = (u16*) 0x6000000;

// set pixel (r,c) to a color
void setPixel(int r, int c, u16 color) {
	videoBuffer[OFFSET(r, c, 240)] = color;
}

// draw an image using dma
void drawImage(const unsigned short arr[]) {
	DMA[3].src = arr;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240*160) | DMA_ON;
}

void drawImage3(int r, int c, int width, int height, const u16* image) {
	for (int i = 0; i < height; i++) {
		DMA[3].src = &image[OFFSET(i, 0, width)];
		DMA[3].dst = videoBuffer + OFFSET(r+i, c, 240);
		DMA[3].cnt = width | DMA_ON;
	}
}

PLAYER drawPlayer(PLAYER player, int frame) {
	//CHECK FACE HERE
 
	if (player.facing == RIGHT) {
	    //CHECK STANCE HERE
		if (player.stance == DASH) {
		    	drawImage3(player.row, player.col, GOKU_DASH1_WIDTH, GOKU_DASH1_HEIGHT, goku_dash1);
			player.height = GOKU_DASH1_HEIGHT;
		    	player.width = GOKU_DASH1_WIDTH;    
		} else if (player.stance == JUMP) {
			drawImage3(player.row, player.col, GOKU_JUMP_WIDTH, GOKU_JUMP_HEIGHT, goku_jump);
		 	player.height = GOKU_JUMP_HEIGHT;
			player.width = GOKU_JUMP_WIDTH; 
		} else if (player.stance == RUN) {
			if (frame == 0) {
		    		drawImage3(player.row, player.col, GOKU_RUN1_WIDTH, GOKU_RUN1_HEIGHT, goku_run1);
		          	player.height = GOKU_RUN1_HEIGHT;
				player.width = GOKU_RUN1_WIDTH; 
			} else if(frame == 1){
				drawImage3(player.row, player.col, GOKU_RUN2_WIDTH, GOKU_RUN2_HEIGHT, goku_run2);
		     		player.height = GOKU_RUN2_HEIGHT;
				player.width = GOKU_RUN2_WIDTH; 

			} else if(frame == 2){
				drawImage3(player.row, player.col, GOKU_RUN3_WIDTH, GOKU_RUN3_HEIGHT, goku_run3);
		   		player.height = GOKU_RUN3_HEIGHT;
				player.width = GOKU_RUN3_WIDTH; 
			}
		} else if (player.stance == STAND) {
			drawImage3(player.row, player.col, GOKU_STAND_WIDTH, GOKU_STAND_HEIGHT, goku_stand);
			player.height = GOKU_STAND_HEIGHT;
			player.width = GOKU_STAND_WIDTH; 
		} else if (player.stance == DOWN) {
			drawImage3(player.row, player.col, GOKU_DOWN_WIDTH, GOKU_DOWN_HEIGHT, goku_down);
			player.height = GOKU_DOWN_HEIGHT;
			player.width = GOKU_DOWN_WIDTH;
		}
	} else {
		 if (player.stance == DASH) {
		 	drawImage3(player.row, player.col, GOKU_DASH1L_WIDTH, GOKU_DASH1L_HEIGHT, goku_dash1L);
		 	player.height = GOKU_DASH1L_HEIGHT;
			player.width = GOKU_DASH1L_WIDTH; 
		} else if (player.stance == JUMP) {
			drawImage3(player.row, player.col, GOKU_JUMP_WIDTH, GOKU_JUMP_HEIGHT, goku_jumpL);
			player.height = GOKU_JUMP_HEIGHT;
			player.width = GOKU_JUMP_WIDTH; 
		} else if (player.stance == RUN) {
			if (frame == 0) {
		    		drawImage3(player.row, player.col, GOKU_RUN1L_WIDTH, GOKU_RUN1L_HEIGHT, goku_run1L);
		           	player.height = GOKU_RUN1L_HEIGHT;
		    		player.width = GOKU_RUN1L_WIDTH; 
			} else if(frame == 1){
				drawImage3(player.row, player.col, GOKU_RUN2L_WIDTH, GOKU_RUN2L_HEIGHT, goku_run2L);
		   		player.height = GOKU_RUN2L_HEIGHT;
		   		player.width = GOKU_RUN2L_WIDTH; 

			} else if(frame == 2){
				drawImage3(player.row, player.col, GOKU_RUN3L_WIDTH, GOKU_RUN3L_HEIGHT, goku_run3L);
		    		player.height = GOKU_RUN3L_HEIGHT;
		    		player.width = GOKU_RUN3L_WIDTH; 
			}
		} else if (player.stance == STAND) {
		    	drawImage3(player.row, player.col, GOKU_STAND_WIDTH, GOKU_STAND_HEIGHT, goku_standL);
			player.height = GOKU_STAND_HEIGHT;
		   	player.width = GOKU_STAND_WIDTH; 
		} else if (player.stance == DOWN) {
			drawImage3(player.row, player.col, GOKU_DOWN_WIDTH, GOKU_DOWN_HEIGHT, goku_downL);
			player.height = GOKU_DOWN_HEIGHT;
			player.width = GOKU_DOWN_WIDTH;
		}
	}
    return player; 
} 

void drawPlatform (PLATFORM platform) {
	if (platform.facing == 0) {
		drawImage3(platform.row, platform.col, PLATFORM_UP_WIDTH, PLATFORM_UP_HEIGHT, platform_up);
		platform.height = PLATFORM_UP_HEIGHT;
		platform.width = PLATFORM_UP_WIDTH;
	}
	else if (platform.facing == 1) {
		drawImage3(platform.row, platform.col, PLATFORM_LEFT_WIDTH, PLATFORM_LEFT_HEIGHT, platform_left);
		platform.height = PLATFORM_LEFT_HEIGHT;
		platform.width = PLATFORM_LEFT_WIDTH;
	}
	else if (platform.facing == 2) {
		drawImage3(platform.row, platform.col, PLATFORM_RIGHT_WIDTH, PLATFORM_RIGHT_HEIGHT, platform_right);
		platform.height = PLATFORM_RIGHT_HEIGHT;
		platform.width = PLATFORM_RIGHT_WIDTH;
	}
}
void setColor(volatile u16 color) {
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240*160) | DMA_SOURCE_FIXED | DMA_ON;
}

void clearScreen() {
	setColor(BLACK);
}

// A function to draw a FILLED rectangle starting at (r, c).
void drawRect(int r, int c, int width, int height, u16 color) {
    for (int i = 0; i < height; i++)
	{
		DMA[3].src = &color;
		DMA[3].dst = videoBuffer + OFFSET(r+i, c, 240);
		DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
	}
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}
int checkCollision(PLAYER player, PLATFORM platform, int scenario) {
	switch(scenario) {
		case 1:
			if ( (player.row <= (platform.row + platform.height)) &&	//case bonking from bottom
			((player.col + player.width) >= platform.col) &&
			(player.col <= (platform.col + platform.width)) ) {
			return 1;
   			}
		case 2:
			 if ( ((player.row + player.height) >= platform.row) &&	//case dropping in from top
			((player.col + player.width) >= platform.col) &&
			(player.col <= (platform.col + platform.width)) ) {
			return 2;
			}
		default:
			return 0;
	} 
    return 0;
}

