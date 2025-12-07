#include "../drivers/frame_buffer.h"
#include "../drivers/interrupts.h"

void kmain()
{
    interrupts_install_idt();
    enable_hardware_interrupts();

    while (1) { }
}
