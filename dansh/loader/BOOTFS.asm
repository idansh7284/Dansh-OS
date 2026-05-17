BITS 32
ORG 0

dd 0xDDFF

name:
	db "Dansh", 0
	times 16-($-name) db 0

dd 133
dd 1
dd 0xFFDD

