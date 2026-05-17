[org 0x7c00]
[bits 16]
; offest + (16 * segmuent) = address
; 0x0000 + (16 * 0x07E0)
; 0x0000 + 7E00
; = 7E00
; 0x7E00 + (16 * 0x0000) = 7E00


; ========================== (CODE) ==========================
bootloader:
    cli
    ; set value for dl in driver 
    mov [drive_num_var], dl 

    ; set 0
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; mini msg
    mov si, info_msg
    call Print
	;restart disk
    mov ah, 0
    mov dl, [drive_num_var]
    int 0x13
	; stage 2
    mov ax, 0x07E0      ; set eax 0x07E0[]
    mov es, ax
    xor bx, bx          ; ES:BX = 0x07E0:0x0000 = 0x7E00
	
    mov ah, 0x02        ; function read
    mov al, 32          ; read 128 sector
    mov ch, 0           ; Cylinder 0
    mov dh, 0           ; Head 0
    mov cl, 4           ; sector 2
    mov dl, [drive_num_var]
    int 0x13
    jc load_error
    jmp 0x07E0:0x0000

load_error:
    mov si, error_msg
    call Print
    jmp $
; ========================== (FUNCTIONS) ==========================	
Print:
	mov ah, 0x0E   ; mode simple in bios
.next_char:
	lodsb		   ; si++
	cmp al, 0      ; if al = "\0" : end for string
	je .close	   ; if al = 0 jmp to return
	int 10h		   ; interrupte 10h in bios : print char
	jmp .next_char
.close:
	ret           ; return
; ========================== (DATA) ==========================
drive_num_var db 0
info_msg db "loader20426 virsion 0.1", 13, 10, "Booting from HDD...", 13, 10, 0
error_msg db "Error loading Stage 2", 0

times 510-($-$$) db 0
dw 0xAA55
