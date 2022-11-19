//
//  Samuel M - sam-chami
//  ds_calc - A calculator for the Nintendo DS or DSi.
//  https://xamuel.xyz/ds_calc
//

//
//  Tokenization is a W.I.P.
//

#include <nds.h>
#include <stdio.h>
#include <math.h>

// Import keypad image
#include "keypad.h"

bool inRange(int low, int high, int x);
int operate(int termA, int termB, int operation);

int main(void)
{
	// Declarations
	touchPosition touch;
	int buttons[23][5] = { {0, 1, 3 ,4, 7},		//
			       {0, 5, 3, 8, 4},		//  0-9 = number
			       {0, 10, 3, 12, 1},	//  -1 = dot
                               {0, 14, 3, 17, 0},	//  -11 = pi
                               {4, 1, 7, 4, 8},		//  -2 = substract
                               {4, 5, 7, 8, 5},		//  -3 = add
                               {4, 10, 7, 12, 2},	//  -4 = multiply
                               {4, 14, 7, 17, -1},      //  -5 = divide
			       {8, 1, 10, 4, 9},        //  -6 = power
                               {8, 5, 10, 8, 6},        //  -7 = root
                               {8, 10, 10, 12, 3},      //  -8 = log 
                               {8, 14, 10, 17, -11},     //  -9 = backspace
			       {12, 1, 15, 4, -5},      //  -10 = equal
                               {12, 5, 15, 8, -4},      //
                               {12, 10, 15, 12, -3},
                               {12, 14, 15, 17, -2},
			       {16, 1, 18, 4, -6},
                               {16, 5, 18, 8, -7},
                               {16, 10, 18, 12, -8},
			       {19, 1, 22, 4, -9},
                               {19, 5, 22, 8, 0},
                               {19, 10, 22, 12, 0},
                               {16, 14, 22, 17, -10},
                             };

	int termA = 0;
	int termB = 0;
	bool firstDigit = true;
	bool termCurrent = true; // Determines the current term (true = A, false = B)
	int operation; // The operation we are performing (+, -, x, ^)
	int result = 0;

	// Set the Sub Display to bitmap background mode
	videoSetModeSub(MODE_5_2D);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	
	// Set the Main Display to Text Mode
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	consoleInit(0, 0, BgType_Text4bpp, BgSize_T_256x256, 16, 6, true, true);
	
	// Display some Info
	iprintf("\x1b[2;3HNintendo DS(i) calculator");
        iprintf("\x1b[3;3Hxamuel.xyz");
        iprintf("\x1b[18;3HResult:");
	
	// draw keypad bitmap
	int bg2 = bgInitSub(2, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(keypadBitmap, bgGetGfxPtr(bg2), 256*192);
	dmaCopy(keypadPal, BG_PALETTE_SUB, 256*2);
	
	// main loop
	while(1) {
	   swiWaitForVBlank();
	   touchRead(&touch);
	   
	   int i;
	   for (i = 0; i < 23; i++) {
	      if (inRange(buttons[i][0], buttons[i][2], touch.px/10) && inRange(buttons[i][1], buttons[i][3], touch.py/10)) {
	      	 if (buttons[i][4] < 0) {
		    if (inRange(-2, -8, buttons[i][4])) {
			operation = buttons[i][4];
			termCurrent = false;
			firstDigit = true;
		    } else {
		       switch(buttons[i][4]) {
		          case -1: // dot
			     break;
			  case -9: // remove
                             if (termCurrent) {
                                termA = 0;
                             } else {
                                termB = 0;
                       	     }
			     break;
			  case -10: // equal
			     result = operate(termA, termB, operation);
			     if (result != NULL) {
			        iprintf("\x1b[18;24H%d", result);
			     } else {
                                iprintf("\x1b[18;15HERROR");
			     }
                             //termA = result;
			     //termB = 0;
			     //operation = 0;
			     firstDigit = true;
			     termCurrent = true;
			     break;
		       }
		    }
		 } else {
		       if (termCurrent) {
		          if (firstDigit){
			     termA = buttons[i][4];
			     firstDigit = false;
			  } else {
			     termA = termA * 10 + buttons[i][4];
			  }
		       } else {
		          if (firstDigit){
                             termB = buttons[i][4];
			     firstDigit = false;
                          } else {
                             termB = termB * 10 + buttons[i][4];
                          }
		       }
		 }
	      }
	   }

           iprintf("\x1b[8;5H%d, %d, %d\n", termA, termB, touch.z1);
	}

	return 0;
}

// Determines if a value (x) is in a specific range
bool inRange(int low, int high, int x) {
	return ((x-high)*(x-low) <= 0);
}

int operate(int termA, int termB, int operation) {
	int result = 0;

	switch (operation) {
	   case -2:
	      result = termA - termB;
	      break;
	   case -3:
              result = termA + termB;
	      break;
	   case -4:
	      result = termA * termB;
              break;
	   case -5:
	      if (termB != 0){
	         result = termA / termB;
	      } else {
	         result = NULL;
	      }
	      break;
           case -6:
	      result = pow(termA, termB);
              break;
           case -7:
	      result = pow(termB, 1.0 / termA); // The importance of mathematics: n√x=x^1/n,
              break;
	   case -8:
	      result = log10(termB) / log10(termA); // The importance of Mathematics II: logₐ x= log₁₀ x/log₁₀ a
              break;
	}

	return result;
}

