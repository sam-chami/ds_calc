
//{{BLOCK(keypad)

//======================================================================
//
//	keypad, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ bitmap not compressed
//	Total size: 512 + 65536 = 66048
//
//	Time-stamp: 2022-11-18, 19:51:07
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_KEYPAD_H
#define GRIT_KEYPAD_H

#define keypadBitmapLen 65536
extern const unsigned int keypadBitmap[16384];

#define keypadPalLen 512
extern const unsigned short keypadPal[256];

#endif // GRIT_KEYPAD_H

//}}BLOCK(keypad)
