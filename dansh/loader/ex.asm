bits 32
org 0x100000

start:
    mov [0xb8500], byte 'k'
    mov [0xb8501], byte 0x0F
    mov [0xb8502], byte 'e'
    mov [0xb8503], byte 0x0F
	mov [0xb8504], byte 'r'
    mov [0xb8505], byte 0x0F
	mov [0xb8506], byte 'n'
    mov [0xb8507], byte 0x0F
    mov [0xb8508], byte 'e'
    mov [0xb8509], byte 0x0F
	mov [0xb850A], byte 'l'
    mov [0xb850B], byte 0x0F
hang:
    jmp hang