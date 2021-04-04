asm_src := $(shell find src -name *.asm)
asm_obj := $(patsubst src/%.asm, build/%.o, $(asm_src))

all: build_kernel

$(asm_obj): $(asm_src)
	nasm -f elf64 $(patsubst build/%.o, src/%.asm, $@) -o $@

.PHONY: build_kernel run clean
build_kernel: $(asm_obj)
	ld -n -o build/kernel.bin -T linker.ld $^ && \
	cp build/kernel.bin iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o build/kernel.iso iso

run: build/kernel.iso
	qemu-system-x86_64 -cdrom $^

clean:
	rm build/*
