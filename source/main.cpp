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
#include <unordered_map>

// Import keypad image
#include "keypad.h"


bool inRange(int low, int high, int x);
float operate(float termA, float termB, int operation);

using namespace std;
int main(void)
{
	//      //
        // OBJC //
        //      //

	// Related to touch:
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
	// Related to operating
	float termA = 0.00;
	unsigned int precisionA = 0;
	float termB = 0.00;
	unsigned int precisionB = 0;
	bool firstDigit = true;
	bool termCurrent = true; // Determines the current term (true = A, false = B)
	int operation; // The operation we are performing (+, -, x, ^)
	float result = 0.00;
	unsigned int decimal = 0;
	bool opFinished = false;

	// other
	char coperator;
	unordered_map<int, char> operators { { -2, '-'}, { -3, '+'}, { -4, 'x'}, { -5, '/'}, { -6, '^'}, { -7, 'r'}, { -8, 'l'} };
	//      //
	// CODE //
	//      //

	// Set the Sub Display to bitmap background mode
	videoSetModeSub(MODE_5_2D);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	
	// Set the Main Display to Text Mode
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	
	consoleInit(0, 0, BgType_Text4bpp, BgSize_T_256x256, 16, 6, true, true);
	
	// Display some Info
	iprintf("\x1b[2;3HDS_calc");
	iprintf("\x1b[3;3Hxamuel.xyz/dscalc");
        iprintf("\x1b[2;%dH%d%%", 27 - 3, getBatteryLevel());
	
	// draw keypad bitmap
	int bg2 = bgInitSub(2, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(keypadBitmap, bgGetGfxPtr(bg2), 256*192);
	dmaCopy(keypadPal, BG_PALETTE_SUB, 256*2);
	
	// main loop
	while(1) {
	   swiWaitForVBlank();
	   touchRead(&touch);
	   scanKeys();

	   int i;
	   for (i = 0; i < 23; i++) {
	      if (inRange(buttons[i][0], buttons[i][2], touch.px/10) && inRange(buttons[i][1], buttons[i][3], touch.py/10)) {
	      	 if (buttons[i][4] < 0) {
		    if (inRange(-2, -8, buttons[i][4])) {
		       operation = buttons[i][4];
		       if (termCurrent) { precisionA = decimal;}
                       else { precisionB = decimal;}
		       termCurrent = false;
		       firstDigit = true;
		       decimal = 0;
		       coperator = operators[buttons[i][4]];
		    } else {
		       switch(buttons[i][4]) {
		          case -1: // dot
			     decimal = 1;
			     break;
			  case -9: // remove
                             termA = 0.00;
                             termB = 0.00;
			     result = 0.00;
			     precisionA = 0;
                             precisionB = 0;
			     firstDigit = true;
                             termCurrent = true;
			     iprintf("\x1b[18;2H                              ");
			     iprintf("\x1b[20;1H                               ");
			     printf("\x1b[18;3H%.*f %c %.*f =", 0, termA, coperator, 0, termB);
                             printf("\x1b[20;23H0.000");
			     break;
			  case -10: // equal
			     //if (termCurrent) { precisionA = decimal;}
			     result = operate(termA, termB, operation);
			     iprintf("\x1b[18;2H                              ");
			     printf("\x1b[18;3H%.*f %c %.*f =", precisionA, termA, coperator, decimal, termB);
			     if (result != NULL) {
				int digits = log10(result)+1;
			        iprintf("\x1b[20;1H                               ");
				printf("\x1b[20;%dH%.4f", 22 - digits, result);
			     } else {
                                iprintf("\x1b[18;23;HERROR");
			     }
                             precisionA = 0;
			     precisionB = 0;
			     decimal = 0;
			     firstDigit = true;
			     termCurrent = true;
			     opFinished = true;
			     break;
			  case -11:
			     if (termCurrent) { termA = 3.141593; }
			     else { termB = 3.141593; }
			     decimal = 6;
			     break;
		       }
		    }
		 } else {
		    if (keysDown()) {
			opFinished = false;
		        if (decimal > 0) {
			   if (termCurrent) {
                              termA = termA + buttons[i][4] / pow(10, decimal);
			      decimal++;
                           } else {
                              termB = termB + buttons[i][4] / pow(10, decimal);
                       	      decimal++;
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
		 if (!opFinished){
		    iprintf("\x1b[18;2H                              ");
		    if (precisionA == 0 && termCurrent) {
		       printf("\x1b[18;3H%.*f %c %.*f =", decimal, termA, coperator, 0, termB);
		    } if (precisionB == 0 && !termCurrent) {
		       printf("\x1b[18;3H%.*f %c %.*f =", precisionA, termA, coperator, decimal, termB);
		    }
		 }
	      }
	   }
	}

	return 0;
}

// Determines if a value (x) is in a specific range
bool inRange(int low, int high, int x) {
	return ((x-high)*(x-low) <= 0);
}

float operate(float termA, float termB, int operation) {
	float result = 0.00;

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
	      if (termB != 0.00){
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

