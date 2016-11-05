#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
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

void drawPlayer(PLAYER player) {
	//CHECK FACE HERE
    if (player.facing == RIGHT) {
	    //CHECK STANCE HERE
        if (player.stance == DASH) {
            drawImage3(player.row, player.col, GOKU_DASH4_WIDTH, GOKU_DASH4_HEIGHT, goku_dash4);
	        player.height = GOKU_DASH4_HEIGHT;
            player.width = GOKU_DASH4_WIDTH;    
        }
        else if (player.stance == JUMP) {
            drawImage3(player.row, player.col, GOKU_JUMP1_WIDTH, GOKU_JUMP1_HEIGHT, goku_jump1);
	        player.height = GOKU_JUMP1_HEIGHT;
            player.width = GOKU_JUMP1_WIDTH; 
        }
        else if (player.stance == RUN) {
            drawImage3(player.row, player.col, GOKU_RUN1_WIDTH, GOKU_RUN1_HEIGHT, goku_run1);
	        player.height = GOKU_RUN1_HEIGHT;
            player.width = GOKU_RUN1_WIDTH; 
        } 
        else if (player.stance == STAND) {
            drawImage3(player.row, player.col, GOKU_STAND_WIDTH, GOKU_STAND_HEIGHT, goku_stand);
	        player.height = GOKU_STAND_HEIGHT;
            player.width = GOKU_STAND_WIDTH; 
        }
    } else {
	 if (player.stance == DASH) {
            drawImage3(player.row, player.col, GOKU_DASH4L_WIDTH, GOKU_DASH4L_HEIGHT, goku_dash4L);
            player.height = GOKU_DASH4L_HEIGHT;
            player.width = GOKU_DASH4L_WIDTH; 
        }
        else if (player.stance == JUMP) {
            drawImage3(player.row, player.col, GOKU_JUMP1_WIDTH, GOKU_JUMP1_HEIGHT, goku_jump1L);
	        player.height = GOKU_JUMP1_HEIGHT;
            player.width = GOKU_JUMP1_WIDTH; 
        }
        else if (player.stance == RUN) {
            drawImage3(player.row, player.col, GOKU_RUN1_WIDTH, GOKU_RUN1_HEIGHT, goku_run1L);
	        player.height = GOKU_RUN1_HEIGHT;
            player.width = GOKU_RUN1_WIDTH; 
        } 
        else if (player.stance == STAND) {
            drawImage3(player.row, player.col, GOKU_STAND_WIDTH, GOKU_STAND_HEIGHT, goku_standL);
	        player.height = GOKU_STAND_HEIGHT;
            player.width = GOKU_STAND_WIDTH; 
        }
	} 
} 

void drawPlatform (PLATFORM platform) {
	if (platform.facing == 0) {
		drawImage3(platform.row, platform.col, PLATFORM_UP_WIDTH, PLATFORM_UP_HEIGHT, platform_up);
	}
	else if (platform.facing == 1) {
		drawImage3(platform.row, platform.col, PLATFORM_LEFT_WIDTH, PLATFORM_LEFT_HEIGHT, platform_left);
	}
	else if (platform.facing == 2) {
		drawImage3(platform.row, platform.col, PLATFORM_RIGHT_WIDTH, PLATFORM_RIGHT_HEIGHT, platform_right);
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

// A function to draw a HOLLOW rectangle starting at (r, c).
void drawHollowRect(int r, int c, int width, int height, u16 color) {
	int row, col;
	row = 0;
	for (col = 0; col < width; col++) {
		setPixel(r+row, c+col, color);
	}

	col = 0;
	for (row = 1; row < height - 1; row++) {
		setPixel(r+row, c+col, color);
	}

	col = width - 1;
	for (row = 1; row < height - 1; row++) {
		setPixel(r+row, c+col, color);
	}


    row = height - 1;
	for (col = 0; col < width; col++) {
		setPixel(r+row, c+col, color);
	}
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}
int checkCollision(PLAYER player, PLATFORM platform) {
//case bonking from bottom
	if ( (player.row <= (platform.row + platform.height)) && //head
		((player.col + player.width) >= platform.col) &&  //right side
		(player.col <= platform.col + platform.width) ){  //left side
		return 1;
	}
	return 0;
}

/*//int collisionBufferZone
int collision(int aRow, int aCol, int aWidth, int aHeight, int bRow, int bCol, int bWidth, int bHeight)
{
	if (aRow < (bRow + bHeight) && (aRow + aHeight) > bRow)
	{
		if (aCol < (bCol + bWidth) && (aCol + aWidth) > bCol)
		{
			return 1;
		}
	}
	return 0;
}
}*/
