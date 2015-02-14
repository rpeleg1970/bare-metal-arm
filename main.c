#include "mem/alloc.h"
#include "io/stdio.h"
#include "ds/rb_tree.h"
#include "hook/hook.h"

static int ssize; /* goes into .bss */
static char message[] = "size of hello string is: ";
static char hello[] = "hello ARM bare-metal C and ASM";
static char nl[] = "\r\n";


int _strlen(const char *s)
{
  int i;
  for (i=0; s[i]!=0x0; i++);
  return i;
}

int main()
{
  _uart0_prints(hello);
  _uart0_prints(nl);
  _uart0_prints(message);
  ssize = _strlen(hello);
  _uart0_printi(ssize);
  _uart0_prints(nl);

  mem_test();
  rbt_test();
  orig_func(2,3);
  hook(orig_func,hook_func);
  orig_func(2,3);

  _uart0_prints("bye.\n");
}
