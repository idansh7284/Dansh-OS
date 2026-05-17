/*
	-----------------------
	* DANSH OS beta 1.00
	* DANSH LAODER 1.0
	* 32-bit mode / BOIS 
	* ATA DRIVER 1.00
	***********************
	Type file : source file (C-lang)
	1:28 PM
	/04/2026
	***********************

*/

#include <io.h>
#include <ATA.h>
// https://wiki.osdev.org/ATA_PIO_Mode
enum {
	ATA_Data            = 0x1f0,
	ATA_Features        = 0x1f1,
	ATA_sector          = 0x1f2,
	ATA_LBA_low         = 0x1f3,
	ATA_LBA_mid         = 0x1f4,
	ATA_LBA_high        = 0x1f5,
	ATA_Drive           = 0x1f6,
	ATA_Command         = 0x1f7,
	ATA_Status          = 0x1f7
};


int read(unsigned int lba, unsigned int adress) {
	outb(ATA_sector, 1);
	outb(ATA_LBA_low, lba & 0xFF);
	outb(ATA_LBA_mid, (lba >> 8) & 0xFF);
	outb(ATA_LBA_high, (lba >> 16) & 0xFF);
	outb(ATA_Drive, 0xE0 | ((lba >> 24) & 0x0F));
	outb(ATA_Command, 0x20);

	while (1) {
		unsigned char stat = inb(ATA_Status);
		if ((stat & 0b10000000) != 0) {
			continue;
		}
		else if ((stat & 0b0000001) !=0) {
			return 1;
		}
		else if ((stat & 0b0001000) != 0) {
			break;
		}
	}
	unsigned short *buffer = (unsigned short *)adress;
	for (int count = 0; count < 256; count++) {
		buffer[count] = inw(ATA_Data);
	}
	return 0;
}

ata_form ata = {
	.read = read
};