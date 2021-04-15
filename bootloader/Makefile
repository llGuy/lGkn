CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -c -fno-pie -fno-PIC
CC = gcc

all: main.o boot.o
	ld main.o boot.o -no-PIE -melf_i386 -n -o kernel.bin -T linker.ld

main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

boot.o: boot32_large.asm
	nasm -f elf boot32_large.asm -o boot.o
