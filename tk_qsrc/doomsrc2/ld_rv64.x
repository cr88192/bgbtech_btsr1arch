ENTRY(_start)

SECTIONS
{
. = 0x1100000;
.text : { *(.text) }
. = 0x1200000;
.data : { *(.data) }
.bss : { *(.bss) }
}