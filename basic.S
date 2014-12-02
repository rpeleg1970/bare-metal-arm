.text
entry: b start
arr:   .byte 1,4,9
eoa:
.align
start:
  ldr r0,=eoa 
  ldr r1,=arr
  mov r3, #0
loop:
  ldrb r2,[r1]
  add r1,r1,#1
  add r3,r2,r3
  cmp r1,r0
  bne loop
stop: b stop
