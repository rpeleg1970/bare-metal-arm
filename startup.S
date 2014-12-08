.section "vectors" @@ exception vector
reset:  b start
undef:  b undef
swi:    b swi
pabt:   b pabt
reserv: nop
irq:    b irq
fiq:    b fiq

.text
start:
        @@ copy data to RAM
        ldr r0, =flash_sdata
        ldr r1, =ram_sdata
        ldr r2, =data_size

        @@ skip if data_size==0
        cmp r2, #0
        beq init_bss

copy:
        ldrb r4, [r0], #1
        strb r4, [r1], #1
        subs r2, r2, #1
        bne copy

init_bss:
        @@ initialize BSS with zero values (unitialized globals in C)
        ldr r0, =sbss
        ldr r1, =ebss
        ldr r2, =bss_size

        @@ skip if bss_size==0
        cmp r0, r1
        beq init_stack

        mov r4, #0
zero:
        strb r4, [r0], #1
        cmp r0, r1
        bne zero

init_stack:
        @@ place the stack at the bottom of RAM, currently 1MB from bottom see also .ld file
        ldr sp, =stack_base

        @@ done - branch to main
        bl  main

stop:   b stop


