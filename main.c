#include "mem/alloc.h"

static int ssize; /* goes into .bss */
static char message[] = "size of hello string is: ";
static char hello[] = "hello ARM bare-metal C and ASM";
static char nl[] = "\r\n";
static char hx[] = "0x";

extern void char_uart0(char c);

void _uart0_prints(const char *s)
{
  int i;
  for (i=0; s[i]!=0x0; i++)
    char_uart0(s[i]);
}

void _uart0_printi(unsigned int n)
{
  int i=0;
  char crs[10]; /* max 32bit int is 10 digits */
  do {
    crs[i++] = (char)(n%10)+'0';
    n = n/10;
  } while (n>0);

  /* reverse result digits */
  while(--i>=0)
    char_uart0(crs[i]);
}

void _uart0_printx(unsigned int n)
{
  int i=0;
  char c;
  char crs[8]; /* max 32bit int is 8 hex digits */
  do {
    c = (char)(n%16); 
    if(c<10) {
      crs[i++] = c+'0';
    } else {
      crs[i++] = c-10+'A';
    }
    n = n/16;
  } while (n>0);

  /* reverse result digits */
  _uart0_prints(hx);
  while(--i>=0)
    char_uart0(crs[i]);
}

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

  _uart0_prints("bye.");
}
