CC=i686-elf-gcc
CFLAGS=-ffreestanding -g -std=gnu99 -Wall -Wextra -O3
OBJS=start.o kernel.o gdt.o
QEMU= qemu-system-i386
QEMU_FLAGS=-kernel


all: kernel.elf

gdt.o: gdt.c start.o
	${CC} ${CFLAGS} -c $? -o $@

start.o: start.s
	${CC} ${CFLAGS} -c $? -o $@

kernel.o: kernel.c
	${CC} ${CFLAGS} -c $? -o $@

kernel.elf: linker.ld ${OBJS}
	${CC} ${CFLAGS} -nostdlib -T $< ${OBJS} -o kernel.elf -lgcc

run: kernel.elf
	${QEMU} ${QEMU_FLAGS} $<

clean:
	rm -f $(wildcard *.o *.elf)
