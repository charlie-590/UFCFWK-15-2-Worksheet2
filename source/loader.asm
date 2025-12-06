global loader
extern kmain

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER

section .text
align 4

    ; multiboot header required by GRUB
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:
    ; set up the stack before calling C code
    mov esp, kernel_stack + KERNEL_STACK_SIZE

    ; jump to C from assembly
    call kmain

.hang:
    ; loop forever after returning from kmain
    jmp .hang         

KERNEL_STACK_SIZE equ 4096

section .bss
align 4
kernel_stack:
    resb KERNEL_STACK_SIZE

