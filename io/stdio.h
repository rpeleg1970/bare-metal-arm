#ifndef __stdio_h_
#define __stdio_h_

/* TBA actual stdio funcitons... for now exposing the low level UART print */


void _uart0_prints(const char *s);
void _uart0_printi(unsigned int n);
void _uart0_printx(unsigned int n);

#endif
