CC=i686-elf-gcc
CFLAGS=-ffreestanding -g
OBJS=start.o kernel.o
QEMU= qemu-system-i386
QEMU_FLAGS=-kernel


all: kernel.elf

start.o: start.s
	${CC} ${CFLAGS} -std=gnu99 -c $? -o $@

kernel.o: kernel.c
	${CC} ${CFLAGS} -std=gnu99 -c $? -o $@

kernel.elf: linker.ld ${OBJS}
	${CC} ${CFLAGS} -nostdlib -T $< ${OBJS} -o kernel.elf -lgcc

run: kernel.elf
	${QEMU} ${QEMU_FLAGS} $<

clean:
	rm -f $(wildcard *.o *.elf)
