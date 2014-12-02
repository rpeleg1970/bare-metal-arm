.global _entry
.text
_entry: b start
str: 
.asciz "what's up doc?"
.equ nul,0
.align
start:  
  ldr r0, =str
  mov r1, #0
  ldr r3, =uart
loop:
  ldrb r2, [r0], #1
  cmp r2, #nul
  beq stop
  strb r2,[r3,#0]
  add r1,r1,#1
  b loop
stop:   b stop

.section uart
uart0: .byte 0x0
