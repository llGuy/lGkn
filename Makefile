asm_src := $(shell find src -name *.asm)
asm_obj := $(patsubst src/%.asm, build/%.o, $(asm_src))

c_src := $(shell find src -name *.c)
c_obj := $(patsubst src/%.c, build/%.o, $(c_src))

CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c

all: build_kernel

$(c_obj): $(c_src)
	$(CC) $(CFLAGS) $(patsubst build/%.o, src/%.c, $@) -o $@

$(asm_obj): $(asm_src)
	nasm -f elf $(patsubst build/%.o, src/%.asm, $@) -o $@

.PHONY: build_kernel run clean
build_kernel: $(asm_obj) $(c_obj)
	ld -melf_i386 -n -o build/kernel.bin -T linker.ld $^ && \
	cp build/kernel.bin iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o build/kernel.iso iso

run: build/kernel.iso
	qemu-system-x86_64 -cdrom $^

clean:
	rm build/*
