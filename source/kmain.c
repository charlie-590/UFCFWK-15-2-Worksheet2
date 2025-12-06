#include "../drivers/framebuffer.h"

void kmain()
{
    fb_clear();
    fb_move(0, 0);
    fb_write("hi");

    while (1) { }
}
