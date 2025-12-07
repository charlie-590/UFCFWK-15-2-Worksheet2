CC = gcc
AS = nasm
LD = ld

CFLAGS  = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostdinc
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T source/link.ld

OBJ = \
source/loader.o \
source/kmain.o \
drivers/frame_buffer.o \
drivers/ports.o \
drivers/keyboard.o \
drivers/interrupts.o \
drivers/pic.o \
drivers/hardware_interrupt_enabler.o \
drivers/interrupt_asm.o \
drivers/interrupt_handlers.o

all: kernel.elf os.iso

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

kernel.elf: $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-A os \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o os.iso \
		iso

run: os.iso
	qemu-system-i386 -boot d -cdrom os.iso -m 32

clean:
	rm -f $(OBJ) kernel.elf os.iso
