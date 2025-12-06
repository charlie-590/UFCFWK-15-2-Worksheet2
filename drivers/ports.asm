global outb

section .text
outb:
    mov dx, [esp + 4]      ; port
    mov al, [esp + 8]      ; value
    out dx, al
    ret
