#include "alloc.h"
#include "io/stdio.h"

void mem_test()
{
  void *p[256];
  int i;


  for(i=0; i<256; i++)
  {
    p[i] = malloc(8+i); /* first 8 will align to 16, then to 32, etc. */
    _uart0_printx((unsigned int)p[i]);
    _uart0_prints(" (size=");
    _uart0_printi(*(unsigned int*)(p[i]-sizeof(size_t)));
    _uart0_prints(")\n");
  }

  for(i=0; i<256; i+=2)
    free(p[i]); /* free only half of the blocks */
  mem_dump_dot();

  for(i=0; i<256; i++)
  {
    p[i] = malloc(8+i); /* first 8 will align to 16, then to 32, etc. */
    _uart0_printx((unsigned int)p[i]);
    _uart0_prints(" (size=");
    _uart0_printi(*(unsigned int*)(p[i]-sizeof(size_t)));
    _uart0_prints(")\n");
  }
  mem_dump_dot();

}
