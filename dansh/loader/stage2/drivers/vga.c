#include <vga.h>
#include <io.h>

unsigned char volatile *address_text_mode = (unsigned char*)0xB8000; 

// *************************************
/*
	* hardwar tools
	1 - out byte
	2 - in byte
*/

// *************************************
//  	STRUCTS AND VARIBLES 

struct {
	int x;
	int y;
	unsigned char color;
}coord;

// *************************************
//		FUNCTIONS AND LOGIC

// -----------------
// time : 21:15
// date : 15/04/2026
// -----------------
int init() {
	coord.x = 0;
	coord.y = 0;
	coord.color = 0x07;
	return 0;
}

// -----------------
// time : 21:20
// date : 15/04/2026
// -----------------
/*
Some concepts may be complex for beginners, but I will try to explain them to them in the future.
*/
void putchar(char byte) {
	// index or pointer in ram :
	// y * hith 
	// result + x
	// result * 2 (2 byte; byte for text; byte for color)

	int index = ((coord.y * 80) + coord.x) * 2;
	if (byte == '\n') {
		// a new line
		coord.y++;  // y = y + 1
		coord.x = 0;// x = 0
	}
	else {
		if (coord.x >= 80) {
			coord.x = 0;
			coord.y++;
		}
		address_text_mode[index] = byte;
		index++;
		address_text_mode[index] = coord.color;
		coord.x++;
	}
	if (coord.y >= 25) {
		for (int i = 0; i < 24 * 80; i++) {
            address_text_mode[i * 2] = address_text_mode[(i + 80) * 2];
            address_text_mode[i * 2 + 1] = address_text_mode[(i + 80) * 2 + 1];
        }
        for (int i = 24 * 80; i < 25 * 80; i++) {
            address_text_mode[i * 2] = ' ';
            address_text_mode[i * 2 + 1] = coord.color;
        }
        coord.y = 24;
	}
}

// -----------------
// time : 21:27
// date : 15/04/2026
// -----------------
void setcolor(unsigned char color) {
	coord.color = color; // copy new color in real varible for color
}

// -----------------
// time : 21:35
// date : 15/04/2026
// -----------------
void pstr(char *text) {

	for (int i = 0; text[i] != '\0'; i++) {
		putchar(text[i]); // display byte 
	}
}

// -----------------
// time : 21:20
// date : 16/04/2026
// -----------------
/*
Today, April 16th, 2026, marks the anniversary of Knowledge Day. May God have mercy on Abdelhamid Ben Badis.

*/
void clear() {
	// 80 * 25 * 2 = 4000
	//i + 2 byte
	// 1 byte = byte
	// 1 byte = color
	for (int i = 0; i < 4000; i += 2) { // 0x00b8000 + 0x0000002
		address_text_mode[i] = ' ';
		address_text_mode[i + 1] = coord.color;
	}
	coord.x = 0; // in start 0xb800
	coord.y = 0;
}

// -----------------
// time : 15:45
// date : 17/04/2026
// -----------------

void removechar() {
	int index = ((coord.y * 80) + coord.x) * 2;

	address_text_mode[index - 1] = 0x00; //
	address_text_mode[index - 2] = ' '; // set \0 ;

	if (coord.x > 0) { // if x > start line
		coord.x--; // x - 1
	}else {
		coord.x = 79; // the end for line before
		coord.y--;  // line - 1
	}
	
}

void disable_cursor() {
	outb(0x3D4, 0x0A); // vga port 0x3D4
	outb(0x3D5, 0x20);
}


void enable_cursor () {}

vga_form console = {
	.init           = init,
	.putchar        = putchar,
	.setcolor       = setcolor,
	.print          = pstr,
	.clear          = clear,
	.removechar     = removechar,
	.disable_cursor = disable_cursor,
	.enable_cursor  = enable_cursor
};