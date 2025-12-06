# Worksheet 2 - Part 1
## Overview 
In this Worksheet we had to implement basic input/output using the VGA framebuffer, following Chapter 4 of The Little Book of OS Development. This was achieved by loading the kernel using GRUB and transferring control from assembly to C, and producing text output directly from hardware interaction.

# Task 1 - booting a tiny OS
First I created the directory structure described in the worksheet and writing the kernel in assembler.

Following the worksheet I created loader.asm which is the intial kernel which contained:
* A multiboot header 
* A entry label loader used as the kernel entry point
* And mov eax 0xCAFEBABE, which is a test that writes cafebabe to the eax register.

![alt text](images/CAFEBABE_snip.png)

Then the assembler file was complied using the NASM specified in the worksheet: nasm -f elf32 loader.asm
A linker script called link.ld was made as shown in the worksheet with:
* ENTRY(loader)
* And the kernel loaded at the address 0x00100000 1MB
The kernel was linked using ld producing kernel.elf which GRUB loads into memory.

Then I downloaded Stage2_eltorito and placed it in the grub directory and with all the files in placed, I generated the .iso image using the following command: 
genisoimage -R\
-b boot/grub/stage2_eltorito\
-no-emul-boot\
-boot-load-size 4\
-A os\
-input-charset utf8\
-quiet\
-boot-info-table\
-o os.iso\
iso

Finally, to test that the ISO was running correctly I ran it using qemu and checked the logQ.txt. As you can see below CAFEBABE was found in logQ.txt many times 

![alt text](images/task1_cafebabe_log.png)

## Task 2 - Calling C from Assembler
In Task 2 I had to extend the kernel so that control can be transferred from assembler to C.

the assembler loader was modified to:
* Set up a stack manually
* And Call the C function Kmain

The Makefile was updated to:
* compile C source files in source/ 
* And link them together with the assembler object files 

Below you can see the OS being launch using these new changes:

![alt text](images/task2_grub_kernel_boot.png)

## Task 3 - Simple I/O using the Framebuffer
In Task 3 I had to add a basic I/O, using the information from chapter 4 of the OS book.

The VGA text framebuffer is accessed using memory-mapped I/O at the address 0x000B8000.

![alt text](images/framebuffer_address_code.png)
Each screen cell consists of:
* One byte for the ASCII character 
* And one byte for the colour attribute.

![alt text](images/framebuffer_attribute_code.png)
A framebuffer was implemented in the driver/ directory. writing directly to this address allows text to be displayed on the OS screen.

Some hardware devices require I/O ports rather than memory-mapped access. The framebuffer cursor is one example. 
An outb routine was added in assembly to write to the I/O ports 0x3D4 and 0x3D5, allowing the hardware cursor position to be updated. 

Finally, to complete Task 3, the framebuffer was extended to expose a full API meaning:
* The cursor movement uses a 2D interface,
* Writing strings to the screen,
* clearing the framebuffer,
* supporting the text colour via attribute bytes.
The kernel uses this API to test the functionality by clearing the screen and printing text.

![alt text](images/task3_framebuffer_output.png)

## QEMU/ Exectution method
While following the worksheet instructions on the csctcloud environment, QEMU produced display-related errors when attempting to use VGA output.
The first error I encounterd was "qemu-system-i386: -curses: invalid option" so i wasnt able to use this option.
Then the second error I encountered was without using the -curses but it still resulted in the error "gtk initialization failed" 

Becuase of these errors I decided to download qemu-w64 on my local device and download my os.iso image off the cloud environment and I ran it locally using this qemu command: "qemu-system-i386 -boot d -cdrom os.iso -m 32" which provided the correct output of "hi" in the OS!

