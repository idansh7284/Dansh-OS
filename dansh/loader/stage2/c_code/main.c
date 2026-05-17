 
// includes
#include <vga.h>
#include <ATA.h>
#include <BFS.h>

void main() {
	console.disable_cursor();
	console.setcolor(0x0); // color blue
	console.clear(); // clean screen
	console.setcolor(0x0F); // color white
	console.print("\nDANSH loader 1.0 \n");
	BFS();
	while (1) {
	asm ("hlt");
	}
}