ENTRY(_entry)
SECTIONS
{
  . = 0x00000;
  .startup . : { add.o(.text) }
  .text : { *(.text) }
  .data : { *(.data) }
  .bss : { *(.bss COMMON) }
  . = ALIGN(8);
  . = . + 0x1000; /* 4kB of stack memory */
  stack_top = .;
  . = 0x40100000;
  uart : { add.o(.uart) }
}
