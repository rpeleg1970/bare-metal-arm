#include "mem/alloc.h"
#include "io/stdio.h"
#include "ds/rb_tree.h"

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
  void *p[4];
  int i;

  _uart0_prints(hello);
  _uart0_prints(nl);
  _uart0_prints(message);
  ssize = _strlen(hello);
  _uart0_printi(ssize);
  _uart0_prints(nl);

  /* simple malloc/free test: allocate 2 of 2 alignment sizes, release,
   * allocate again. since free works as stack - second run we allocated a 32
   * byte chunk for malloc 11/12, then we are stuck with 2 free chunks which are
   * too small for 13/14, so we allocated from uncharted */
  for(i=0; i<4; i++)
  {
    p[i] = malloc(11+i); /* first 2 will aling to 16, next 2 to 32 */
    _uart0_printx((unsigned int)p[i]);
    _uart0_prints(nl);
  }

  for(i=0; i<4; i++)
    free(p[i]);

  for(i=0; i<4; i++)
  {
    p[i] = malloc(11+i); /* first 2 will aling to 16, next 2 to 32 */
    _uart0_printx((unsigned int)p[i]);
    _uart0_prints(nl);
  }

  rbt_test();

  _uart0_prints("bye.");
}
