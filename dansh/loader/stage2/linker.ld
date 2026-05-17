ENTRY(entry)

SECTIONS
{
    . = 0x7E00;
    _stage2_start = .;
    .text : {
   	output/stage2/stage2_asm.o(.text)
    *(.text*)}
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) }
    _stage2_end = .;
}
