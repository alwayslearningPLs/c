# Learning memory model of ASM

## Memory mapping

The memory of a computer can be considered an array of bytes.

In modern CPUs, there are hardware mapping registers which are used to give each process a _protected address space_.
This means that multiple people can each run a program which starts at address _0x4004c8_ at the same time.
Theses processes perceive the same "logical" addresses, while they are using memory at different "physical" addresses.

The hardware mapping registers on an x86-64 CPU can map pages of 2 different sizes:

- 4096 bytes
- 2 MB

Linux uses 2 MB pages for the kernel and 4 KB for other uses.

The operation of the memory system is to translate the upper bits of the address from a process's logical address to a physical address.

Suppose a reference is made to logical address `0x4000002220`. Since 4096 is 2^12, the offset within the page is the right-most _12_ bits (_0x220_).
The page number is the rest of the bits `0x4000002`. A hardware register (or multiple registers) translates this page number to a physical page address, let's say `0x780000000`. Then the two addresses are combined to get the physical address `0x780000220`.

You only get a segmentation fault when your logical address reaches far enough past the end of the array to cause the CPU to reference a page table entry which is not mapped into your process.

### Process memory model in Linux

In Linux memory for a process is divided into 4 logical regions:

- `text`
- `data`
- `heap`
- `stack`

The stack is mapped to the highest address of a process and on x86-64 Linux this is `0x7fffffffffff` or _131 TB_. This address is selected based on the max number of bits allowed in logical addresses being _48 bits_. This address is 47 bits of all 1 bits. The decision was made to not use bit 48, since canonical addresses have to extend bit 48 through bits 49-63.

```
 -----------   131TB
|           |
|   stack   | Restricted by the kernel to 16 MB or 0x1000000, which means that if the top is 0x7fffffffffff, then the start point is 0x7fffff000000.
|           |
|           |
|   heap    | Limit is imposed by the sum of physical memory and swap space.
|           |
 -----------
|           | .bss segment (Block Started by Symbol). data which is statically allocated in a process, but is not stored in the exec file.
|   data    | 
|           | .data segment
 -----------
|           |
|   text    |
|           |
 -----------    0
```

It appears that the _lowest_ address in an x86-64 process is _0x400000_

## Debugging with GDB

We are going to use the print (p) and examine (x) commands.

`p <expression>` or `p/<FMT> <expression>` where FMT is a single letter defining the format of data to print:

| letter | format |
| ------ | ------ |
| d | decimal (default) |
| x | hexadecimal |
| t | binary |
| u | unsigned |
| f | floating point |
| i | instruction |
| c | character |
| s | string |
| a | address |

Some output
```gdb
(gdb) p/d &a
$4 = 4202496
(gdb) p/x &a
$5 = 0x402000
(gdb) p/t &a
$6 = 10000000010000000000000
(gdb) p/u &a
$7 = 4202496
(gdb) p/f &a
$8 = 2.0763089003852552e-317
(gdb) p/c &a
$9 = 0 '\000'
(gdb) p/s &a
$10 = (<data variable, no debug info> *) 0x402000
(gdb) p/a &a
$11 = 0x402000
```

`x/NFS <address>` where:

- N is a number of items to print (default 1).
- F is a single letter format as used in the print command.
- S is the size of each memory location.

| letter | size | bytes |
| ------ | ---- | ----- |
| b | byte | 1 |
| h | halfword | 2 |
| w | word | 4 |
| g | giant | 8 |

