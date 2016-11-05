#include "mylib.h"
#include "./images/goku_stand.h"
#include "./images/goku_standL.h"
#include "./images/goku_run1.h"
#include "./images/goku_run1L.h"
#include "./images/goku_jump1.h"
#include "./images/goku_jump1L.h"
#include "./images/goku_dash4.h"
#include "./images/goku_dash4L.h"
 
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
	for (int i = 0; i < height; i++)
	{
		DMA[3].src = &image[OFFSET(i, 0, width)];
		DMA[3].dst = videoBuffer + OFFSET(r+i, c, 240);
		DMA[3].cnt = width | DMA_ON;
	}
}

void drawPlayer(PLAYER player) {
	//CHECK FACE HERE
    if (player.facing == RIGHT){
	    //CHECK STANCE HERE
        if (player.stance == DASH) {
            drawImage3(player.row, player.col, GOKU_DASH4_WIDTH, GOKU_DASH4_HEIGHT, goku_dash4);
	    }
        else if (player.stance == JUMP) {
            drawImage3(player.row, player.col, GOKU_JUMP1_WIDTH, GOKU_JUMP1_HEIGHT, goku_jump1);
	    }
        else if (player.stance == RUN) {
            drawImage3(player.row, player.col, GOKU_RUN1_WIDTH, GOKU_RUN1_HEIGHT, goku_run1);
	    } 
        else if (player.stance == STAND) {
            drawImage3(player.row, player.col, GOKU_STAND_WIDTH, GOKU_STAND_HEIGHT, goku_stand);
	    }
    } else {
	 if (player.stance == DASH) {
            drawImage3(player.row, player.col, GOKU_DASH4L_WIDTH, GOKU_DASH4L_HEIGHT, goku_dash4L);
	    }
        else if (player.stance == JUMP) {
            drawImage3(player.row, player.col, GOKU_JUMP1_WIDTH, GOKU_JUMP1_HEIGHT, goku_jump1L);
	    }
        else if (player.stance == RUN) {
            drawImage3(player.row, player.col, GOKU_RUN1_WIDTH, GOKU_RUN1_HEIGHT, goku_run1L);
	    } 
        else if (player.stance == STAND) {
            drawImage3(player.row, player.col, GOKU_STAND_WIDTH, GOKU_STAND_HEIGHT, goku_standL);
	    }
	} 
} 

void drawPlatform (PLATFORM platform) {
	for(int i = 0; i < platform.num; i++) {
		drawImage3(platform.row, platform.col, , GOKU_STAND_HEIGHT, goku_standL);
}
void setColor(volatile u16 color)
{
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240*160) | DMA_SOURCE_FIXED | DMA_ON;
}

void clearScreen()
{
	setColor(BLACK);
}

// A function to draw a FILLED rectangle starting at (r, c).
void drawRect(int r, int c, int width, int height, u16 color)
{
    for (int i = 0; i < height; i++)
	{
		DMA[3].src = &color;
		DMA[3].dst = videoBuffer + OFFSET(r+i, c, 240);
		DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
	}
}

// A function to draw a HOLLOW rectangle starting at (r, c).
void drawHollowRect(int r, int c, int width, int height, u16 color)
{
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

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
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

int enemyCollision(DOODLER doodler, ENEMY theEnemy)
{
	return collision(doodler.row, doodler.col, DOODLER_SIZE, DOODLER_SIZE, theEnemy.row, theEnemy.col, ENEMY_SIZE, ENEMY_SIZE);
}

int jetpackCollision(DOODLER doodler, JETPACK theJetpack)
{
	return collision(doodler.row, doodler.col, DOODLER_SIZE, DOODLER_SIZE, theJetpack.row, theJetpack.col, JETPACK_WIDTH, JETPACK_HEIGHT);
}*/
