/*
static int sum;
static int arr[] = { 1, 10, 4, 5, 6, 7 };
static const int n = sizeof(arr) / sizeof(arr[0]);
*/
static int ssize; /* goes into .bss */
static char message[] = "size of hello string is: ";
static char hello[] = "hello ARM bare-metal C and ASM";
static char nl[] = "\r\n";

extern void char_uart0(char c);

void _uart0_prints(const char *s)
{
  int i;
  for (i=0; s[i]!=0x0; i++)
    char_uart0(s[i]);
}

void _uart0_printi(int n)
{
  int i=0;
  char crs[10]; /* max 32bit int is 10 chars */
  do {
    crs[i++] = (char)(n%10+48);
    n = n/10;
  } while (n>0);

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
  /*
  int i;

  for (i = 0; i < n; i++)
    sum += arr[i];
  */
  _uart0_prints(hello);
  _uart0_prints(nl);
  _uart0_prints(message);
  ssize = _strlen(hello);
  _uart0_printi(ssize);
  _uart0_prints(nl);
  _uart0_prints("bye.");
}
