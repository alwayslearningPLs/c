    segment .data
a   dd      4
b   dd      4.4
c   times   10 dd 0
d   dw      1, 2
e   db      0xfb
f   db      "Hello world", 0 ; strings in yasm don't end in null bytes

    segment .bss
g   resd    1                ; resd means REServe Double word
h   resd    10               ; reserve 10 double words
i   resb    100              ; resb means REServe Byte. In this case, reserve 100 bytes.

    segment .text
    global  _start           ; let the linker know about main
_start:
    push    rbp         ; set up a stack frame for main.
                        ; rbp means register base pointer (start of stack)
    mov     rbp, rsp    ; set rbp to point to the stack frame.
                        ; Register Stack Pointer (current location in stack)
    sub     rsp, 16     ; leave some room for local variable
                        ; leave rsp on a 16 byte boundary
    xor     eax, eax    ; set rax to 0 for return value.
                        ; 011 ^ 011 = 0000
    leave               ; undo the stack frame manipulations
    ret
