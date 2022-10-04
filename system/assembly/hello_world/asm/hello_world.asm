    global _start

    section .text
  _start:
    mov     rax, 1              ; System call to write
    mov     rdi, 1              ; stdout
    mov     rsi, message        ; the content that we want to output
    mov     rdx, 13             ; length of the content
    syscall
    mov     rax, 60             ; exit syscall
    xor     rdi, rdi            ; exit code 0
    syscall
    
    section .data
  message db "Hello World!", 10 ; the value that we are going to print.
