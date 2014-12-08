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
  _uart0_prints(hello);
  _uart0_prints(nl);
  _uart0_prints(message);
  ssize = _strlen(hello);
  _uart0_printi(ssize);
  _uart0_prints(nl);
  _uart0_printx((int)(malloc(10)));
  _uart0_prints(nl);
  _uart0_printx((int)(malloc(10)));
  _uart0_prints(nl);
  _uart0_prints("bye.");
}
