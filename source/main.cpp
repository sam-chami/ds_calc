#include <nds.h>
#include <stdio.h>

// Import keypad image
#include "keypad.h"

int main(void)
{
    // set the mode for 2 text layers and two extended background layers
	videoSetModeSub(MODE_5_2D);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);

	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	//bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0, 0);

	consoleInit(0, 0, BgType_Text4bpp, BgSize_T_256x256, 16, 6, true, true);
	
	iprintf("\n\n\tNintendo DS(i) calculator\n");
        iprintf("\txamuel.xyz\n\n\n\n\n\n\n\n\n");
        iprintf("\n\n\n\n\n\n\n\tResult:");
	
	iprintf("               10");

	int bg2 = bgInitSub(2, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(keypadBitmap, bgGetGfxPtr(bg2), 256*192);
	dmaCopy(keypadPal, BG_PALETTE_SUB, 256*2);
	
    while(1)swiWaitForVBlank();

	return 0;
}

