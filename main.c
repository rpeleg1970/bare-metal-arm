#include "mem/alloc.h"
#include "io/stdio.h"
#include "ds/rb_tree.h"
#include "hook/hook.h"
#include "add_numbers.h" // Add this line

extern void set_neg_value(int *address, int value);

static int ssize; /* goes into .bss */
static char message[] = "size of hello string is: ";
static char hello[] = "hello ARM bare-metal C and ASM";
static char nl[] = "\r\n";
static int test_value = 10;
static uint32_t result_add_asm = 0; // Add this line


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

  _uart0_prints("Original test_value: ");
  _uart0_printi(test_value);
  _uart0_prints(nl);

  set_neg_value(&test_value, 5); // Call assembly function

  _uart0_prints("test_value after set_neg_value: ");
  _uart0_printi(test_value);
  _uart0_prints(nl);

  // Add assembly addition test
  add_and_store(15, 25, &result_add_asm);
  _uart0_prints("Result of assembly addition (15 + 25): ");
  _uart0_printi(result_add_asm);
  _uart0_prints(nl);

  _uart0_prints("bye.\n");
}
