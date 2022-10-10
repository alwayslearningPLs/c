# Learning memory mapping in 64 bit mode

_Control Register 3_ (CR3) is a pointer to the top level of a hierarchical collection of tables in memory which define the translation from virtual addresses
to physical addresses.

Somewhere in the kernel of the OS, an initial hierarchy of the translation tables is prepared and _CR3_ is filled with the address of the top level table ("Page Map Level 4" or _PML4_) in the hierarchy. 

## Page Map Level 4

A virtual address can be broken into fields like this:

- 63-48: unused.
- 47-39: PML4 index.
- 38-30: Page Directory Pointer Index.
- 29-21: Page Directory Index.
- 20-12: Page Table Index.
- 11-0 : Page offset.

The result of the translation process will be a physical address like _0x7fffff008000_ which is combined with the offset (let's say it was _0x1f0_ to yield a physical address of _0x7fffff0081f0_).
