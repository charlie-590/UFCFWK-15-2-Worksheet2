global outb
global inb
global outw     

section .text
outb:
    mov dx, [esp + 4]      ; port
    mov al, [esp + 8]      ; value
    out dx, al
    ret

inb:
    mov dx, [esp + 4]
    in  al, dx
    ret

outw:
    mov dx, [esp + 4]      ; port (16 bits)
    mov ax, [esp + 8]      ; value (16 bits)
    out dx, ax
    ret
