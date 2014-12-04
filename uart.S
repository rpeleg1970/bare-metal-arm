.text
.global char_uart0
char_uart0:
  ldr r4, =0x40100000
  strb r0,[r4,#0]
  bx lr

