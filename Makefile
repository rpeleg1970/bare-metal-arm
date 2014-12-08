# makefile for add
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
GCC=arm-none-eabi-gcc
OC=arm-none-eabi-objcopy
GDB=arm-none-eabi-gdb
QEMU=qemu-system-arm

ASFLAGS=--gstabs+
LDFLAGS=-T connex.ld
GCCFLAGS=-nostdlib -g
OCFLAGS=-O binary

all: main

clean:
	rm *.o *.bin *.elf

dbg-server:
	$(QEMU) -M connex -nographic -pflash flash.bin -gdb tcp::1234 -S

dbg-client:
	$(GDB) *.elf

main: main.elf
	$(OC) $(OCFLAGS) main.elf main.bin
	dd if=/dev/zero of=flash.bin bs=4096 count=4096
	dd if=main.bin of=flash.bin bs=4096 conv=notrunc

main.elf: *.o
	$(GCC) $(GCCFLAGS) $(LDFLAGS) -o main.elf *.o *.c mem/*.c


*.o:
	$(AS) $(ASFLAGS) -o startup.o startup.S
	$(AS) $(ASFLAGS) -o uart.o uart.S
