# makefile for add
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
OC=arm-none-eabi-objcopy
GDB=arm-none-eabi-gdb
QEMU=qemu-system-arm

ASFLAGS=--gstabs+
LDFLAGS=-T add.ld
OCFLAGS=-O binary

all: add

clean:
	rm add.bin add.o add.elf flash.bin

dbg-server:
	$(QEMU) -M connex -nographic -pflash flash.bin -gdb tcp::1234 -S

dbg-client:
	$(GDB) add.elf

add: add.elf
	$(OC) $(OCFLAGS) add.elf add.bin
	dd if=/dev/zero of=flash.bin bs=4096 count=4096
	dd if=add.bin of=flash.bin bs=4096 conv=notrunc

add.elf: add.o
	$(LD) $(LDFLAGS) -o add.elf add.o

add.o:
	$(AS) $(ASFLAGS) -o add.o add.S
