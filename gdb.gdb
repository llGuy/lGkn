target remote localhost:1234
add-symbol-file build/kernel.bin
hbreak kernel_main
continue