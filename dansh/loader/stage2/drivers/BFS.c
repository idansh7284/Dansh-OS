 
/*

	-----------------------
	* iDANSH OS beta 1.00
	* iDANSH LAODER 1.0
	* 32-bit mode / BOIS 
	* BOOT FILE SYSTEM DRIVER 1.00
	***********************
	Type file : source file (C-lang)
	1:28 PM
	15/04/2026
	***********************
*/
#include <vga.h>
#include <ATA.h>
struct BOOT_FS_FORM{
	// cheksum == 0xDDFF
	unsigned int cheksum1;
	// kernel name : 16byte
	char name[16];
	// location start in HDD
	unsigned int sector_start;
	// size for sector
	unsigned int sector_size;
	// cheksum 2 == 0xFFDD
	unsigned int cheksum2;
	// 512 - 32 = 480byte
	unsigned char voi[480];
}__attribute__((packed)); // not align or add size  

// STRUCT for boot file system
struct BOOT_FS_FORM BOOT_FS_DATA;

int BFS() {
	console.print("*****************\n");
	int address = (unsigned int)&BOOT_FS_DATA;
	if (ata.read(1, address) == 1) {
		return 1;
	}
	if (BOOT_FS_DATA.cheksum1 != 0xDDFF || BOOT_FS_DATA.cheksum2 != 0xFFDD) {
		console.setcolor(0x04);
		console.print("error : ");
		console.setcolor(0x07);
		console.print("not found chek sum in boot file system");
		return 0;
	}
	console.print("OS name : ");
	console.print(BOOT_FS_DATA.name);
	console.putchar('\n');

	if (BOOT_FS_DATA.sector_start < 132) {
		console.setcolor(0x04);
		console.print("error : ");
		console.setcolor(0x07);
		console.print("sector acess \n");
		return 0;
	}

	if (BOOT_FS_DATA.sector_size == 0) {
		console.setcolor(0x04);
		console.print("error : ");
		console.setcolor(0x07);
		console.print("sector size \n");
		return 0;
	}

	int counter = 0;
	unsigned int offest = 0x100000 ;
	int size = BOOT_FS_DATA.sector_size;
	console.print("load in 1MB offest in RAM \n");
	console.print("load ... \n");

	while (counter < size ) {
		if (ata.read(BOOT_FS_DATA.sector_start + counter, offest) == 0) {
			offest = offest + 0x200;
			counter++;
		}
		else {
			console.setcolor(0x04);
			console.print("error : ");
			console.setcolor(0x07);
			console.print("HDD no read !\n");
			console.print("new read \n");
		}
	}
	console.print("*****************\n");
	console.print("jmp to offest 0x100000 ...");
	asm volatile (".intel_syntax noprefix \n"
		"jmp 0x100000 \n"
		".att_syntax prefix");
	return 0;
}
