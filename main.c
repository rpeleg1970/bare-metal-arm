/*
static int sum;
static int arr[] = { 1, 10, 4, 5, 6, 7 };
static const int n = sizeof(arr) / sizeof(arr[0]);
*/
static int ssize; /* goes into .bss */
static char message[] = "size of array is: ";
static char hello[] = "hello ARM";


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
  ssize = _strlen(message);
}
