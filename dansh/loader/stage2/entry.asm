[BITS 16]

; this function call GDT amd switch 32bit mode
global entry

entry:
	; A20 line active
	in al, 0x92
    or al, 2
    out 0x92, al
    ; load gdt
	lgdt [GDT_data]
	cli ; desactive interupte
	; active bit in reg cr0
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp 0x08:pm


; ---------------------------
; gdt table
GDT_table:
	dq 0
	.code:
		dw 0xFFFF
		dw 0x0000
		db 0x00
		db 0x9A
		db 0xCF
		db 0x00
	.data:
		dw 0xFFFF
		dw 0x0000
		db 0x00
		db 0x92
		db 0xCF
		db 0
GDT_end:

; ---------------------------
; gdt data
GDT_data:
	dw GDT_end - GDT_table - 1
	dd GDT_table

; ***************************
; mode 32 bit
[BITS 32]
extern main
[BITS 32]
extern main
pm:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000 
    call main
    jmp $


