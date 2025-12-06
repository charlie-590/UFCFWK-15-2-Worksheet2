CC=gcc
AS=nasm
LD=ld

CFLAGS=-m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostdinc
ASFLAGS=-f elf32
LDFLAGS=-m elf_i386 -T source/link.ld

OBJS=source/loader.o \
     source/kmain.o \
     drivers/framebuffer.o \
     drivers/ports.o

all: os.iso

source/loader.o: source/loader.asm
	$(AS) $(ASFLAGS) source/loader.asm -o source/loader.o

source/kmain.o: source/kmain.c
	$(CC) $(CFLAGS) -c source/kmain.c -o source/kmain.o

drivers/framebuffer.o: drivers/framebuffer.c
	$(CC) $(CFLAGS) -c drivers/framebuffer.c -o drivers/framebuffer.o

drivers/ports.o: drivers/ports.asm
	$(AS) $(ASFLAGS) drivers/ports.asm -o drivers/ports.o

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.elf

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
	qemu-system-i386 -nographic -boot d -cdrom os.iso -m 32 -d cpu -D logQ.txt

clean:
	rm -f source/*.o drivers/*.o kernel.elf os.iso logQ.txt
