/*
	DSNSH OS (C) 2026
	GPL3
	by : souag ahmed el fatih
	vga libarary 1.00


	The VGA source file is modular; you can modify its implementation and relink it with the kernel to support alternative display drivers

*/

// FORM FOR VGA 
/*
	kernel call print
	print -> vga header file -> jmp address for vga source file

*/

typedef struct {
//  type  name              argments       ;
	int  (*init)            (void)         ;
	void (*putchar)         (char)         ;
	void (*setcolor)        (unsigned char);
	void (*print)           (char *)       ;
	void (*clear)           (void)         ;
	void (*removechar)      (void)         ;
	void (*disable_cursor)  (void)         ;
	void (*enable_cursor)   (void)         ;
}vga_form;

extern vga_form console;