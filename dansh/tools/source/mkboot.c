#include <stdio.h>
#include <string.h>

#define sector 512
#define bootloader 0

FILE *BOOTFS = NULL;

struct {
	char name[16];
	char sector_start[4];
	char sector_size[4];
}kernel;

int main() {
	printf("mkboot TOOLS 2026 (C) \n");
	BOOTFS = fopen("boot.cfg", "r+");
	if (BOOTFS == NULL ) {
		printf("can't open bootfs.cfg !\n");
		return 1;
	}

	fgets(kernel.name, sizeof(kernel.name), BOOTFS);
	printf("kernel name : %s", kernel.name );
	fgets(kernel.sector_start, sizeof(kernel.sector_size), BOOTFS);
	printf("%s", kernel.sector_start);
	fgets(kernel.sector_size, sizeof(kernel.sector_size), BOOTFS);
	printf("%s\n",kernel.sector_size);
	fclose(BOOTFS);
	return 0;
}