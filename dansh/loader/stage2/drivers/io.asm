[BITS 32]

global outb
global inb 
global outw
global inw


outb:
	; STACK : PORT(4BYTE) DATA(4BYTE) RETURN(4BYTE) 
    mov edx, [esp + 4] ; PORT
    mov eax, [esp + 8]  ; DATA
    out dx, al 
    ret

inb:
	; STACK : PORT(4BYTE) RETURN(4BYTE) 
	mov edx, [esp + 4] ; PORT
	xor eax, eax ; CLEAN EAX
	in al, dx
	ret

inw:
	mov edx, [esp + 4]
	xor eax, eax
	in ax, dx
	ret

outw:
	mov edx, [esp + 4]
	mov eax, [esp + 8]
	out dx, ax
	ret