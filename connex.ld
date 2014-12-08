SECTIONS
{
  . = 0x00000000;
  .text : {
    startup.o (vectors);
    * (.text);
  }
  .rodata : {
    * (.rodata );
  }
  flash_sdata = .;
  
  . = 0xA0000000;
  ram_sdata = .;
  .data : AT (flash_sdata) {
    * (.data);
  }
  ram_edata = .;
  data_size = ram_edata - ram_sdata;

  sbss = .;
  .bss : {
    * (.bss);
  }
  ebss = .;
  bss_size = ebss - sbss;

  . = 0xA0100000;
  stack_base = .;
  heap_start = .;

  . = 0xA4000000;
  ram_end = .;
}
