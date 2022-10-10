# Learning generics of assembly

## Integer constants

`+|- <digits> <radix>` where radix can be:

- `H` for hexadecimal.
- `r` for encoded real.
- `q/o` for octal.
- `t/d` for decimal.
- `y/b` for binary.

eg. `26d`

## Directives

- Assist and control assembly process.
- Are also called _pseudo-ops_.
- Not part of the instruction set.
- They change the way code is assembled.

Some directives are:

- `.CODE`: Indicates the start of a code segment.
- `.DATA`: Indicates the start of a data segment.

## Running assembly with yasm

- elf64 is the 64-bit option
- dwarf2 is used to debug with gdb and ddd

steps:

- `yasm -f elf64 -g dwarf2 -l <name>.lst <name>.asm` to build.
- `ld -o <name> <name>.out` to link (needed because of the `_start`).
- `gcc -o <name> <name>.out` to link (needed when the program defines `main` rather than `_start`)

## Memory mapping

The memory of a computer can be considered an array of bytes.

In modern CPUs there are hardware mapping registers which are used to give each process a protected address space. This means that multiple people can each run a program which starts at addres `0x4004c8` at the same time. These processes perceive the same "logical" addresses, while they are using memory at different "physical" addresses.

The hardware mapping registers on an x86-64 CPU can map pages of 2 different sizes - 4096 bytes and 2 megabytes. Linux uses 2 MB pages for the kernel and 4KB pages for most other users. In some of the more recent CPUs there is also support for 1 GB pages.

The operation of memory system is to translate the upper bits of the address from a process's logical address to a physical address.

You only get a segmentation fault when your logical address reaches far enough past the end of the array to cause the CPU to reference a page table entry which is not mapped into your process.

### Process memory model in linux

In Linux memory for a process is divided into 4 logical regions:

- text
- data
- heap
- stack

The stack is mapped to the highest addresss of a process and on x86-64 linux this is `0x7fffffffffff` or 131TB. This address is selected base on the maximum number of bits allowed in logical addresses being 48 bits.

It appears that the lowest address in an x86-64 process is `0x400000`.

The data segment starts with the `.data` segment which contains initialized data. Above that is the `.bss` segment which stands for "Block Started by Symbol". The `.bss` segment contains data which is statically allocated in a process, but is not stored in the exec file.

The stack segment is restricted in size by the Linux Kernel, typically to 16 megabytes.

Giving the top of the stack: `0x7fffffffffff` and the stack size limited to 16 megabytes we see that the lowest valid stack address is `0x7fffff000000`

To see memory used by one of the process, execute `cat /proc/$$/maps`

