#include "io/stdio.h"

/* the below are defined in assembly code */
extern void* trampoline_template;
extern void* tt_orig_code;
extern void* tt_data;
extern void* hook_template;

void orig_func(int a, int b)
{
  _uart0_prints("original func (a+b): ");
  _uart0_printi(a+b);
  _uart0_prints("\n");
}

void hook_func(int a, int b)
{
  _uart0_prints("hook func (a*b): ");
  _uart0_printi(a*b);
  _uart0_prints("\n");
}

void hook(void* orig_fp, void* hook_fp)
{
  /* copy original 2 instructions into the trampoline */
  for(int i=0; i<=7; i++) {
    *((char*)(tt_orig_code+i)) = *((char*)(orig_fp+i));
  }

  /* set the hook in place of the original 2 instructions */
  for(int i=0; i<=7; i++) {
    *((char*)(orig_fp+i)) = *((char*)(hook_template+i));
  }
  *((unsigned int*)(orig_fp+4)) = (unsigned int)trampoline_template;

  /* set the data in the trampoline placeholders */
  *((unsigned int*)(tt_data+4)) = (unsigned int)(orig_fp+8);
  *((unsigned int*)(tt_data+8)) = (unsigned int)(hook_fp);
}
