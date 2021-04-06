# lGkn

A project to learn about kernel development.

![photo](/res/hello_world.png)

## For debugging:

```
qemu-system-i386 -s -S -cdrom build/kernel.iso
```

```
gdb -ix gdb.gdb (or whatever the path to this file is)
(gdb) target remote localhost:1234
(gdb) add-symbol-file build/kernel.bin
(gdb) hbreak main
(gdb) c
```
