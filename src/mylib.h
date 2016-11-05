
typedef unsigned short u16;
typedef unsigned int u32;

// display

extern u16* videoBuffer;
#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006
#define OFFSET(row, col, rowlen)  ((row)*(rowlen)+(col))
#define REG_DISPCNT *(u16*) 0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)


// color
#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define WHITE COLOR(31,31,31)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define GREY COLOR(25, 25, 25)
#define BLACK 0



// buttons
#define BUTTON_A	(1<<0)
#define BUTTON_B	(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT	(1<<5)
#define BUTTON_UP	(1<<6)
#define BUTTON_DOWN	(1<<7)
#define BUTTON_R	(1<<8)
#define BUTTON_L	(1<<9)
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
#define BUTTONS *(volatile unsigned int *)0x4000130

// dma
typedef struct
{
	const volatile void *src;
	volatile void *dst;
	volatile u32 cnt;
} DMAREC;

#define DMA ((volatile DMAREC *)0x040000B0)

/* DMA channel 0 register definitions*/
#define REG_DMA0SAD         *(vu32*)0x40000B0  /* source address*/
#define REG_DMA0DAD         *(vu32*)0x40000B4  /* destination address*/
#define REG_DMA0CNT         *(vu32*)0x40000B8  /* control register*/

/* DMA channel 1 register definitions*/
#define REG_DMA1SAD         *(vu32*)0x40000BC  /* source address*/
#define REG_DMA1DAD         *(vu32*)0x40000C0  /* destination address*/
#define REG_DMA1CNT         *(vu32*)0x40000C4  /* control register*/

/* DMA channel 2 register definitions*/
#define REG_DMA2SAD         *(vu32*)0x40000C8  /* source address*/
#define REG_DMA2DAD         *(vu32*)0x40000CC  /* destination address*/
#define REG_DMA2CNT         *(vu32*)0x40000D0  /* control register*/

/* DMA channel 3 register definitions */
#define REG_DMA3SAD         *(vu32*)0x40000D4   /* source address*/
#define REG_DMA3DAD         *(vu32*)0x40000D8  /* destination address*/
#define REG_DMA3CNT         *(vu32*)0x40000DC  /* control register*/

// define
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

// destination address flags
#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

// source address flags
#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

// flags / how much to copy
#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

// when to dma flags
#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)

#define DMA_ON (1 << 31)

#define START_ON_FIFO_EMPTY 0x30000000


// game
#define INFO_GUTTER_WIDTH 40
#define DOODLER_SIZE DOODLER32_HEIGHT
#define ENEMY_SIZE ENEMY_WIDTH
enum FACING {LEFT, RIGHT, UP};

typedef struct
{
	int row;
	int col;
	enum FACING facing;
	int doubleJump;
	int dash;
} PLAYER;


typedef struct
{
	int row;
	int col;
	int length;
} PLATFORM;


// **Prototypes**
void drawImage(const unsigned short arr[]);
void drawImage3(int r, int c, int width, int height, const u16* image);

void drawPlayer(PLAYER player);
void drawPlatform(PLATFORM platform);

void setColor(u16 color);
void clearScreen();


void setPixel(int r, int c, u16 color);
void drawRect(int r, int c, int width, int height, u16 color);

void drawHollowRect(int r, int c, int width, int height, u16 color);


void waitForVblank();

//int collision(int aRow, int aCol, int aWidth, int aHeight, int bRow, int bCol, int bWidth, int bHeight);
int death(PLAYER player);

